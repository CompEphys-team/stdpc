#include "DataSaver.h"
#include <QDir>
#include <QDateTime>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

DataSaver::DataSaver()
{
}

bool DataSaver::init(dataSavingParams p_, QVector<ChannelIndex> channels)
{
    p = p_;

    QDateTime now = QDateTime::currentDateTime();
    p.fileName.replace("%Y", now.toString("yyyy"))
            .replace("%M", now.toString("MM"))
            .replace("%D", now.toString("dd"))
            .replace("%h", now.toString("HH"))
            .replace("%m", now.toString("mm"))
            .replace("%s", now.toString("ss"));
    QFileInfo fileinfo(p.fileName);
    QDir dir(fileinfo.path());
    if ( !dir.exists() )
        if ( !dir.mkpath(".") )
            return false;

    if ( p.fileName.contains("%n") ) {
        p.fileName.replace("%n", "%1");
        for ( size_t i = 0; true; i++ ) {
            QString name = p.fileName.arg(i, 4, 10, '0');
            if ( QFileInfo(name).exists() ) {
                p.fileName = name;
                break;
            }
        }
    }

    if ( p.isBinary ) {
        if ( !initBinary(channels) )
            return false;
    } else {
        if ( !initAscii(channels) )
            return false;
    }

    // prepare queues
    size_t bufsz (p.savingFreq > 1e3 ? p.savingFreq : 1e3);
    q.resize(channels.size());
    for ( auto &queue : q )
        queue.reset(new CircularFifo<double>(bufsz));

    return true;
}

QString getColumnFileName(int i, ChannelIndex channel)
{
    QString label = channel.isNone ? "time" : channel.toString('_', true);
    return QString("%1_%2.dat").arg(i, 2, 10, '0').arg(label);
}

bool DataSaver::initBinary(QVector<ChannelIndex> channels)
{
    QDataStream::Version streamversion = QDataStream::Qt_DefaultCompiledVersion;
    QFileInfo fileinfo(p.fileName);
    if ( !fileinfo.exists() || !fileinfo.isDir() )
        if ( !QDir(p.fileName).mkpath(".") )
            return false;

    // Write metadata
    QJsonArray columns;
    for ( int i = 0; i < channels.size(); i++ ) {
        QJsonObject obj;
        if ( channels[i].isNone )
            obj = QJsonObject {
                {"type", "Time"},
                {"units", "s"}
            };
        else
            obj = channels[i].toJson();
        obj.insert("file_name", getColumnFileName(i, channels[i]));
        columns.append(obj);
    }
    QJsonObject json
    {
        {"generator", "StdpC"},
        {"record_name", fileinfo.fileName()},
        {"requested_saving_frequency", p.savingFreq},
        {"creation_time", QDateTime::currentDateTime().toString(Qt::ISODate)},
        {"QDataStream_version", streamversion},
        {"byte_order", p.binaryLittleEndian ? "little_endian" : "big_endian"},
        {"precision", p.binaryDoublePrecision ? "double" : "single"},
        {"data_columns", columns}
    };
    QFile json_file(QString("%1/meta.json"));
    if ( !json_file.open(QFile::WriteOnly) )
        return false;
    json_file.write(QJsonDocument(json).toJson());

    // Create separate streams for each data column
    binaryStreams.resize(channels.size());
    binaryFiles.resize(channels.size());
    for ( size_t i = 0; i < binaryStreams.size(); i++ ) {
        QFile *file(QString("%1/%2")
                   .arg(p.fileName)
                   .arg(getColumnFileName(i, channels[i])));
        if ( !file->open(QFile::WriteOnly) )
            return false;

        QDataStream *stream = new QDataStream(file);
        stream->setVersion(streamversion);
        stream->setByteOrder(p.binaryLittleEndian ? QDataStream::LittleEndian : QDataStream::BigEndian);
        stream->setFloatingPointPrecision(p.binaryDoublePrecision ? QDataStream::DoublePrecision : QDataStream::SinglePrecision);

        binaryStreams[i].reset(stream);
        binaryFiles[i].reset(file);
    }

    return true;
}

bool DataSaver::initAscii(QVector<ChannelIndex> channels)
{
    os.open(p.fileName.toLatin1());
    if ( !os.good() || !os.is_open())
        return false;

    os << p.asciiHeaderPrefix;
    for ( ChannelIndex channel : channels ) {
        QString label = channel.isNone ? "Time" : channel.toString('_', true);
        os << '"' << label << '"' << p.asciiSeparator;
    }
    os << (p.asciiCRLF ? "\r\n" : "\n");

    return true;
}


// Consume data pushed to the queues by DCThread on the main thread
void DataSaver::SaveLine()
{
    double data;
    if ( p.isBinary ) {
        for ( size_t i = 0; i < q.size(); i++ ) {
            q[i]->pop(data);
            *binaryStreams[i] << data;
        }
    } else {
        for ( auto &queue : q ) {
            queue->pop(data);
            os << data << p.asciiSeparator;
        }
        os << (p.asciiCRLF ? "\r\n" : "\n");
    }
}


// Closes the file
void DataSaver::EndDataSaving()
{
    // Not a race condition: This slot is called (via a QueuedConnection) on the main thread,
    // so no call to SaveLine() should remain.
    os.close();
    os.clear();
    binaryStreams.clear();
    binaryFiles.clear();
    q.clear();
}
