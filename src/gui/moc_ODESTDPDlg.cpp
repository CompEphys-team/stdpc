/****************************************************************************
** Meta object code from reading C++ file 'ODESTDPDlg.h'
**
** Created: Thu Aug 21 17:34:34 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ODESTDPDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ODESTDPDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ODESTDPDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_ODESTDPDlg[] = {
    "ODESTDPDlg\0"
};

const QMetaObject ODESTDPDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ODESTDPDlg,
      qt_meta_data_ODESTDPDlg, 0 }
};

const QMetaObject *ODESTDPDlg::metaObject() const
{
    return &staticMetaObject;
}

void *ODESTDPDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ODESTDPDlg))
        return static_cast<void*>(const_cast< ODESTDPDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int ODESTDPDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
