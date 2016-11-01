#ifndef DAQOPTS_H
#define DAQOPTS_H

#include "DaqWidget.h"
#include "MainWin.h"
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <functional>

class DaqOptsBase : public QObject
{
    Q_OBJECT

public:
    DaqOptsBase(QWidget *parent) : QObject(parent) {}
    virtual ~DaqOptsBase() {}
    virtual void removeDevice(size_t idx) = 0;

    virtual void importData() = 0;
    virtual void exportData() = 0;

    virtual void setIndex(size_t i) = 0;

    virtual DaqWidget *widget() = 0;
    virtual void regenerateWidget() = 0;

signals:
    void removedModel(ChannelIndex);

protected slots:
    virtual void activeChanged() = 0;
};

template <class DaqDlg>
class DaqOpts : public DaqOptsBase
{
public:
    DaqOpts(QWidget *parent, QString const& label, std::vector<typename DaqDlg::param_type> *params, size_t idx) :
        DaqOptsBase(parent),
        label(label),
        params(params),
        idx(idx)
    {
        if ( idx >= params->size() ) {
            params->resize(idx+1);
            (*params)[idx].active = false;
        }
        (*params)[idx].removed = false;

        QString name;
        QFuture<DeviceStatus> future;
        if ( DaqDlg::isDAQ::value ) {
            QMessageBox loadMsg(QMessageBox::Information, "Loading...",
                                "Initializing hardware ... this may take a while ...");
            loadMsg.setStandardButtons(QMessageBox::NoButton);
            QFutureWatcher<DeviceStatus> watcher;
            connect(&watcher, SIGNAL(finished()), &loadMsg, SLOT(reject()));
            future = QtConcurrent::run(
                        &Devices, &DeviceManager::initSingle, std::ref(name), DeviceManager::Register()[DaqDlg::param_type::daqClass()], idx);
            watcher.setFuture(future);
            loadMsg.exec();
            future.waitForFinished();
        }

        dlg = new DaqDlg(idx, parent);

        regenerateWidget();
        _widget->setLabel(label + " " + QString::number(idx));

        connect(dlg, SIGNAL(channelsChanged()), parent, SIGNAL(channelsChanged()));
        if ( DaqDlg::isDAQ::value ) {
            connect(dlg, SIGNAL(message(QString)), parent, SLOT(DisplayMessage(QString)));
            connect(dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)), parent, SLOT(updateDeviceStatus(DeviceStatus, const QString&)));
            connect(dlg, SIGNAL(CloseToLimit(QString,QString,double,double,double)),
                    parent, SLOT(CloseToLimitWarning(QString, QString, double, double, double)));

            static_cast<MyMainWindow*>(parent)->updateDeviceStatus(future.result(), name);
            _widget->statusChanged(future.result());
        }
    }

    ~DaqOpts()
    {
        delete dlg;
    }

    inline void removeDevice(size_t idx) { Devices.remove(DeviceManager::Register()[DaqDlg::param_type::daqClass()], idx); }

    void importData()
    {
        dlg->importData();
        _widget->active->setChecked(params->at(idx).active);
    }

    void exportData()
    {
        if ( idx >= params->size() )
            params->resize(idx+1);
        (*params)[idx].active = _widget->active->isChecked();
        dlg->exportData();
    }

    void setIndex(size_t i)
    {
        idx = i;
        _widget->setLabel(label + " " + QString::number(idx));
        dlg->setIndex(idx);
    }

    inline DaqWidget *widget() { return _widget; }

    void regenerateWidget()
    {
        _widget = new DaqWidget();
        connect(_widget->params, SIGNAL(clicked(bool)), dlg, SLOT(open()));
        connect(_widget->active, SIGNAL(clicked(bool)), this, SLOT(activeChanged()));
        if ( DaqDlg::isDAQ::value )
            connect(dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)),
                    _widget, SLOT(statusChanged(DeviceStatus)));
    }

    void activeChanged()
    {
        (*params)[idx].active = _widget->active->isChecked();
        dlg->exportData(true);
    }

    QString label;
    std::vector<typename DaqDlg::param_type> *params;
    DaqDlg *dlg;
    size_t idx;
    DaqWidget *_widget;
};

#endif // DAQOPTS_H
