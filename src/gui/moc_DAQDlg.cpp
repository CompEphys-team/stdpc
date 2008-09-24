/****************************************************************************
** Meta object code from reading C++ file 'DAQDlg.h'
**
** Created: Mon Aug 18 17:26:20 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DAQDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DAQDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DAQDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      20,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DAQDlg[] = {
    "DAQDlg\0\0reinitDAQ()\0appear()\0"
};

const QMetaObject DAQDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DAQDlg,
      qt_meta_data_DAQDlg, 0 }
};

const QMetaObject *DAQDlg::metaObject() const
{
    return &staticMetaObject;
}

void *DAQDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DAQDlg))
        return static_cast<void*>(const_cast< DAQDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int DAQDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: reinitDAQ(); break;
        case 1: appear(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DAQDlg::reinitDAQ()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
