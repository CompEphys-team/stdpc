/****************************************************************************
** Meta object code from reading C++ file 'AlphaBetaHHDlg.h'
**
** Created: Thu Aug 21 17:34:25 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AlphaBetaHHDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AlphaBetaHHDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AlphaBetaHHDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x0a,
      41,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AlphaBetaHHDlg[] = {
    "AlphaBetaHHDlg\0\0,\0updateOutChn(int,int*)\0"
    "updateInChn(int,int*)\0"
};

const QMetaObject AlphaBetaHHDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AlphaBetaHHDlg,
      qt_meta_data_AlphaBetaHHDlg, 0 }
};

const QMetaObject *AlphaBetaHHDlg::metaObject() const
{
    return &staticMetaObject;
}

void *AlphaBetaHHDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AlphaBetaHHDlg))
        return static_cast<void*>(const_cast< AlphaBetaHHDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int AlphaBetaHHDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateOutChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 1: updateInChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
