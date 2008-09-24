/****************************************************************************
** Meta object code from reading C++ file 'STDPDlg.h'
**
** Created: Wed Sep 24 11:54:13 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/include/STDPDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'STDPDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_STDPDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_STDPDlg[] = {
    "STDPDlg\0"
};

const QMetaObject STDPDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_STDPDlg,
      qt_meta_data_STDPDlg, 0 }
};

const QMetaObject *STDPDlg::metaObject() const
{
    return &staticMetaObject;
}

void *STDPDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_STDPDlg))
        return static_cast<void*>(const_cast< STDPDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int STDPDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
