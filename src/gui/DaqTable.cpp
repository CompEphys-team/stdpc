#include "DaqTable.h"
#include "DeviceManager.h"
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>

template class DaqOpts<SimulDAQDlg>;
template class DaqOpts<DigiDataDlg>;
#ifdef NATIONAL_INSTRUMENTS
template class DaqOpts<NIDAQDlg>;
#endif

template <class Dlg>
GenericDaqOpts *DaqOpts<Dlg>::create(int idx)
{
    if ( idx >= (int)params->size() ) {
        params->resize(idx+1);
        (*params)[idx].active = false;
    }

    QString name;
    QMessageBox loadMsg(QMessageBox::Information, "Loading...",
                        "Initializing hardware ... this may take a while ...");
    loadMsg.setStandardButtons(QMessageBox::NoButton);
    QFutureWatcher<DeviceStatus> watcher;
    connect(&watcher, SIGNAL(finished()), &loadMsg, SLOT(reject()));
    QFuture<DeviceStatus> future = QtConcurrent::run(
                &Devices, DeviceManager::initSingle<typename Dlg::param_type>, name, idx);
    watcher.setFuture(future);
    loadMsg.exec();
    future.waitForFinished();

    DaqOpts<Dlg> *c = new DaqOpts<Dlg>(parent, _label, params);
    c->idx = idx;
    c->_widget = new DaqWidget();
    c->_widget->setLabel(_label + " " + QString::number(idx));
    c->dlg = new Dlg(idx, parent);
    connect(c->_widget->params, SIGNAL(clicked(bool)), c->dlg, SLOT(open()));
    connect(c->_widget->active, SIGNAL(stateChanged(int)), c, SLOT(activeChanged()));
    connect(c->dlg, SIGNAL(message(QString)), parent, SLOT(DisplayMessage(QString)));
    connect(c->dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)), parent, SLOT(updateDeviceStatus(DeviceStatus, const QString&)));
    connect(c->dlg, SIGNAL(deviceStatusChanged(DeviceStatus, const QString&)), c->_widget, SLOT(statusChanged(DeviceStatus)));
    connect(c->dlg, SIGNAL(channelsChanged()), parent, SIGNAL(channelsChanged()));
    return c;
}

template <class Dlg>
QVector<GenericDaqOpts *> DaqOpts<Dlg>::createAll()
{
    QVector<GenericDaqOpts *> vec;
    vec.reserve(params->size());
    GenericDaqOpts *c;
    for ( size_t i = 0; i < params->size(); i++ ) {
        c = create(i);
        c->importData();
        vec.push_back(c);
    }
    return vec;
}

template <class Dlg>
void DaqOpts<Dlg>::importData()
{
    dlg->importData();
    _widget->active->setChecked(params->at(idx).active);
}

template <class Dlg>
void DaqOpts<Dlg>::exportData()
{
    (*params)[idx].active = _widget->active->isChecked();
    dlg->exportData();
}

template <class Dlg>
void DaqOpts<Dlg>::activeChanged()
{
    (*params)[idx].active = _widget->active->isChecked();
    dlg->exportData(true);
}


DaqTable::DaqTable(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(1);
}

void DaqTable::makeFactory()
{
    factory = new DaqFactoryWidget(this);
    connect(factory->button, SIGNAL(clicked(bool)), this, SLOT(addDaqOpts()));
    for ( GenericDaqOpts *c : proto )
        factory->combo->addItem(c->label());
}

void DaqTable::init(QVector<GenericDaqOpts *> prototypes)
{
    proto = prototypes;
    idx = QVector<int>(proto.size(), 0);
    makeFactory();
    setCellWidget(0, 0, factory);
}

void DaqTable::importData()
{
    clear();
    for ( GenericDaqOpts *c : comp )
        delete c;
    comp.clear();

    int i = 0;
    for ( GenericDaqOpts *p : proto ) {
        QVector<GenericDaqOpts*> comps = p->createAll();
        comp.append(comps);
        idx[i++] = comps.size();
    }
    i = 0;
    setColumnCount(comp.size() + 1);
    for ( GenericDaqOpts *c : comp ) {
        setCellWidget(0, i++, c->widget());
    }

    makeFactory();
    setCellWidget(0, i, factory);
}

void DaqTable::exportData()
{
    for ( GenericDaqOpts *c : comp )
        c->exportData();
}

void DaqTable::addDaqOpts()
{
    int i = factory->combo->currentIndex();
    GenericDaqOpts *c = proto[i]->create(idx[i]++);
    c->exportData();
    insertColumn(comp.size());
    setCellWidget(0, comp.size(), c->widget());
    comp.append(c);
    scrollTo(model()->index(0, comp.size()));
}
