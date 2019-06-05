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

#ifndef OUTPUTCHANNELDLG_H
#define OUTPUTCHANNELDLG_H

#include <QAbstractButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include "ui_OutputChannelDlg.h"
#include "Daq.h"

class OutputChannelDlg : public QDialog, private Ui::OutputChannelDlg
{
     Q_OBJECT

  public:
     OutputChannelDlg(size_t idx, DAQProxy *proxy, QWidget *parent= 0);
     void init();
     void exportData();
     void importData();
     virtual ~OutputChannelDlg();

     inline void setIndex(size_t i) { idx = i; }
     
     int ChnNo;
     int lbNum;

  protected:
     QVector<double> outLow;
     QVector<double> outHigh;
     QVector<QCheckBox *> act;
     QVector<QComboBox *> rng;
     QVector<QLineEdit *> factor;
     QVector<QLineEdit *> bias;
     QVector<QLabel *> allLabel;
     QVector<QCheckBox *> saveChnl;

     size_t idx;
     DAQProxy *proxy;
               
  public slots:
     void accept();
     void reject();
     
  private:
     void clearAll();
}; 

#endif
