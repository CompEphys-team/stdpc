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

#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDataStream>
#include <QFile>
#include <fstream>
#include <memory>
#include "CircularFifo.h"
#include "ObjectDataTypes.h"

class DataSaver : public QObject
{
    Q_OBJECT

private:
    dataSavingParams p;

    std::vector<std::unique_ptr<QDataStream>> binaryStreams;
    std::vector<std::unique_ptr<QFile>> binaryFiles;
    std::ofstream os;

    std::string lineEnding;

protected:
    bool initBinary(QVector<ChannelIndex> channels);
    bool initAscii(QVector<ChannelIndex> channels);

public:
    DataSaver();
    bool init(dataSavingParams p_, QVector<ChannelIndex> channels);

    std::vector<std::unique_ptr<CircularFifo<double>>> q;

public slots:
    void SaveLine();
    void EndDataSaving();
};

#endif // DATASAVER_H
