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

#ifndef INPUTCHANNELDLG_H
#define INPUTCHANNELDLG_H


#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "ui_InputChannelDlg.h"
#include "ObjectDataTypes.h"
#include "Daq.h"

class InputChannelDlg : public QDialog, private Ui::InputChannelDlg
{
     Q_OBJECT

  public:
     InputChannelDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void init();
     void importData();
     virtual ~InputChannelDlg();

     inline void setIndex(size_t i) { idx = i; }
     
     int ChnNo;
     int lbNum;
     
  protected:
     QVector<double> inLow;
     QVector<double> inHigh;
     QVector<QCheckBox *> act;
     QVector<QComboBox *> rng;
     QVector<QLineEdit *> factor;
     QVector<QLabel *> allLabel;
     QVector<QCheckBox *> sDetect;
     QVector<QLineEdit *> SDThresh;
     QVector<QLineEdit *> bias;
     QVector<QCheckBox *> saveChnl;
     QVector<QPushButton *> calib;

     QVector<elecCalibParams> calibBackup;

     size_t idx;
     DAQProxy *proxy;

  public slots:
    void accept();
    void reject();
    void open();

    void exportData();
      
  private:
     void clearAll();
}; 

#endif
