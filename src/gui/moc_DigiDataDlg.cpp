/****************************************************************************
** Meta object code from reading C++ file 'DigiDataDlg.h'
**
** Created: Thu Aug 21 17:34:56 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DigiDataDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DigiDataDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DigiDataDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      22,   12,   12,   12, 0x0a,
      31,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DigiDataDlg[] = {
    "DigiDataDlg\0\0accept()\0reject()\0appear()\0"
};

const QMetaObject DigiDataDlg::staticMetaObject = {
    { &DAQDlg::staticMetaObject, qt_meta_stringdata_DigiDataDlg,
      qt_meta_data_DigiDataDlg, 0 }
};

const QMetaObject *DigiDataDlg::metaObject() const
{
    return &staticMetaObject;
}

void *DigiDataDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DigiDataDlg))
        return static_cast<void*>(const_cast< DigiDataDlg*>(this));
    return DAQDlg::qt_metacast(_clname);
}

int DigiDataDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DAQDlg::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: reject(); break;
        case 2: appear(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
