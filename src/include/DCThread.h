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

#ifndef DCTHREAD_H
#define DCTHREAD_H

#include <QThread>
#include <functional>
#include "Global.h"
#include "Channels.h"
#include "DataSaver.h"

#include "ChannelBufferHelper.h"

class GraphWidget;
class PerformanceMonitor;

class DCThread : public QThread 
{
     Q_OBJECT

 protected:
     void run();
     
 public slots:

private:
     DataSaver *dataSaver;

 public:
     DCThread();
     virtual ~DCThread();
     bool LoadScript(QString &);
     void UnloadScript();

     void setGraph(GraphWidget * = nullptr, double dt = 0.0);
     void setPerformanceMonitor(PerformanceMonitor * = nullptr, double dt = 0.0);

     void setup_and_go();

     inChannel *getInChan(ChannelIndex const& dex);
     outChannel *getOutChan(ChannelIndex const& dex);
     std::vector<ChannelIndex> getChanIndices(ChannelIndex const& dex);

     QVector<const double *> valuesToSave;
     
     bool stopped;
     bool finished;
     bool scripting;
     bool settling;
     bool wakeFromSettling;

     outChnData outNoneData;
     outChannel outChnNone;

     std::shared_ptr<ChannelBufferHelper> bufferHelper;

private:
     bool initial;   
     double t;
     double dt;

     bool saving;
     double savingPeriod;
     double lastWrite;

     QVector<AECChannel*> aecChannels;
     QVector<inChannel*> aecIn;
     QVector<outChannel*> aecOut, aecCopy;

     GraphWidget *graph;
     QVector<const double *> graphVar;
     double graphDt;
     double graphDummy;

     PerformanceMonitor *perfmon;
     double perfDt;

     QList<QPair<double, std::function<void()>>> scriptq;
     QList<QPair<double, std::function<void()>>>::iterator scrIter;
    
 signals:
     void error(QString message);
     void message(QString message);
     void CloseToLimit(QString, QString, double, double, double);
     void updateRate(int Hz);
     void done();
     void saveData();
     void forceSaveData();
};


#endif
