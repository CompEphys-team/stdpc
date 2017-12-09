#include "DataSaver.h"
#include <QDir>
#include <QDateTime>
#include <QRegularExpression>

DataSaver::DataSaver()
{
}

bool DataSaver::init(dataSavingParams p_, QVector<QString> labels)
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
        if ( !initBinary(labels) )
            return false;
    } else {
        if ( !initAscii(labels) )
            return false;
    }

    // prepare queues
    size_t bufsz (p.savingFreq > 1e3 ? p.savingFreq : 1e3);
    q.resize(labels.size());
    for ( auto &queue : q )
        queue.reset(new CircularFifo<double>(bufsz));

    return true;
}

bool DataSaver::initBinary(QVector<QString> labels)
{
    QFileInfo fileinfo(p.fileName);
    if ( !fileinfo.exists() || !fileinfo.isDir() )
        if ( !QDir(p.fileName).mkpath(".") )
            return false;

    // TODO: Metadata

    binaryStreams.resize(labels.size());
    binaryFiles.resize(labels.size());
    for ( size_t i = 0; i < binaryStreams.size(); i++ ) {
        QString label = labels[i].toLatin1();
        label.replace(QRegularExpression("[^-_\\.a-zA-Z0-9]"), "_");
        QFile *file(QString("%1/%2_%3.dat")
                   .arg(p.fileName)
                   .arg(i)
                   .arg(label));
        if ( !file->open(QFile::WriteOnly) )
            return false;

        QDataStream *stream = new QDataStream(file);
        // TODO: Stream attributes

        binaryStreams[i].reset(stream);
        binaryFiles[i].reset(file);
    }

    return true;
}

bool DataSaver::initAscii(QVector<QString> labels)
{
    os.open(p.fileName.toLatin1());
    if ( !os.good() || !os.is_open())
        return false;

    os << "% ";
    for ( QString &label : labels )
        os << label.toLatin1().constData() << " ";
    os << "\n";

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
            os << data << " ";
        }
        os << "\n";
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
