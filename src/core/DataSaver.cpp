/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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

    if ( fileinfo.fileName().contains("%n") ) { // Ignore %n in path
        while ( p.fileName.count("%n") != 1 ) // I'm not dealing with *multiple* running indices!
            p.fileName.remove(p.fileName.indexOf("%n"), 1);
        if ( runningIndex == -1 || p.fileName != indexedFilename ) {
            int pos = fileinfo.fileName().indexOf("%n");
            dir.setNameFilters(QStringList(fileinfo.fileName().replace(pos, 2, "????")));
            dir.setSorting(QDir::Name);
            QStringList existing = dir.entryList();
            int n = 0;
            if ( !existing.isEmpty() ) {
                bool ok = false;
                for ( auto it = existing.rbegin(); !ok && it != existing.rend(); it++ ) {
                    n = it->mid(pos, 4).toInt(&ok) + 1;
                }
                if ( !ok )
                    n = 0;
            }
            indexedFilename = p.fileName;
            runningIndex = n;
        } else {
            runningIndex++;
        }
        p.fileName = p.fileName.replace("%n", "%1").arg(runningIndex, 4, 10, QChar('0'));
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
    return QString("%1_%2.dat").arg(i, 2, 10, QChar('0')).arg(label);
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
    QFile json_file(QString("%1/meta.json").arg(p.fileName));
    if ( !json_file.open(QFile::WriteOnly) )
        return false;
    json_file.write(QJsonDocument(json).toJson());

    // Create separate streams for each data column
    binaryStreams.resize(channels.size());
    binaryFiles.resize(channels.size());
    for ( size_t i = 0; i < binaryStreams.size(); i++ ) {
        QFile *file = new QFile(QString("%1/%2")
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
    os.open(p.fileName.toLatin1(), ios_base::out | ios_base::binary); // text mode auto-converts \n into \r\n...
    if ( !os.good() || !os.is_open())
        return false;

    lineEnding = (p.asciiCRLF ? "\r\n" : "\n");
    p.asciiHeaderPrefix.replace(QRegularExpression("\\r?\\n"), lineEnding.data());
    p.asciiSeparator.replace(QRegularExpression("\\r?\\n"), lineEnding.data());

    os << p.asciiHeaderPrefix.toStdString();
    for ( int i = 0, last = channels.size() - 1; i <= last; i++ ) {
        QString label = channels[i].isNone ? "Time" : channels[i].toString('_', true);
        os << '"' << label << '"';
        if ( i < last )
            os << p.asciiSeparator.toStdString();
    }
    os << lineEnding;

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
        for ( int i = 0, last = q.size()-1; i <= last; i++ ) {
            q[i]->pop(data);
            os << data;
            if ( i < last )
                os << p.asciiSeparator.toStdString();
        }
        os << lineEnding;
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
