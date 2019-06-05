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

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTime>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include "Global.h"
#include "DataSavingDlg.h"
#include "ObjectDataTypes.h"
#include "DeviceManager.h"
#include "DCThread.h"
#include "ChannelListModel.h"

namespace Ui {
class MainWindow;
}

class MyMainWindow : public QMainWindow
{
     Q_OBJECT

  protected:
    void closeEvent(QCloseEvent *event);
    void doExportLog(QString &);

  public:
     MyMainWindow(QWidget *parent= 0);
     virtual ~MyMainWindow();
     void exportData(bool ignoreDAQ = false);
     void importData();

     DataSavingDlg *DSDlg;
     DCThread *DCT;

     ChannelListModel *inChnModel, *outChnModel;
     
  public slots:
     void CloseToLimitWarning(QString, QString, double, double, double);
     void DisplayAbout();
     void DisplayMessage(QString);
     void updateDeviceStatus(DeviceStatus = DeviceStatus::Inactive, const QString & = QString());
     void updateStartButton();

  signals:
     void channelsChanged();
     void modelRemoved(ChannelIndex);
                 
  private slots:
     void StartButClicked();
     void StopButClicked();
     
     void SaveConfig();
     void LoadConfig();
     void ExportLog();
     void ClearLog();
     void SaveProtocol();
     void LoadProtocol();
     void LoadScript();
     void UnLoadScript();
          
 private:
    QFileDialog *ExportLogFileDlg;
    QFileDialog *LoadProtocolFileDlg;
    QFileDialog *SaveProtocolFileDlg;
    QFileDialog *LoadScriptFileDlg;    
    
    QLabel *loadedProtocolStatus;
    QLabel *rateIndicator;

    // adjustable parameter stuff

    void doLoadProtocol(QString &);
    void doSaveProtocol(QString &);

    Ui::MainWindow *ui;

}; 

#endif
