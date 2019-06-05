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

#ifndef ElectrodeCompDlg_H
#define ElectrodeCompDlg_H

#include <QDialog>
#include <QVector>
#include <QMessageBox>
#include "ObjectDataTypes.h"
#include "Calibrator.h"
#include "ChannelListModel.h"


namespace Ui {
    class ElectrodeCompDlg;
}

class ElectrodeCompDlg : public QDialog
{
    Q_OBJECT
    //Q_DISABLE_COPY(ElectrodeCompDlg)

    private:
        elecCalibParams &p;
        ChannelIndex dex;

        ChannelListModel outChnModel;

        bool InitCalibrator();        
        void TimingTest();

        Ui::ElectrodeCompDlg *ui;

    public:                
        Calibrator calibrator;

        ElectrodeCompDlg(elecCalibParams &p, ChannelIndex dex, QWidget *parent = 0);
        ~ElectrodeCompDlg();
        void exportData();
        void importData();

    public slots:
        void MeasureElectrode();
        void MeasureMembrane();
        void CalibrateElectrode();
        void ToggleCompensation();
        
    signals:
        void message(QString message);

};

#endif // ElectrodeCompDlg_H
