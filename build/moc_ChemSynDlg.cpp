/****************************************************************************
** Meta object code from reading C++ file 'ChemSynDlg.h'
**
** Created: Wed Sep 24 11:54:04 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/include/ChemSynDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChemSynDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChemSynDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      32,   11,   11,   11, 0x0a,
      49,   11,   11,   11, 0x0a,
      85,   83,   11,   11, 0x0a,
     108,   83,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ChemSynDlg[] = {
    "ChemSynDlg\0\0PlastMethodChange()\0"
    "STDComboChange()\0ResCloseClicked(QAbstractButton*)\0"
    ",\0updateOutChn(int,int*)\0updateInChn(int,int*)\0"
};

const QMetaObject ChemSynDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ChemSynDlg,
      qt_meta_data_ChemSynDlg, 0 }
};

const QMetaObject *ChemSynDlg::metaObject() const
{
    return &staticMetaObject;
}

void *ChemSynDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChemSynDlg))
        return static_cast<void*>(const_cast< ChemSynDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int ChemSynDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PlastMethodChange(); break;
        case 1: STDComboChange(); break;
        case 2: ResCloseClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 3: updateOutChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 4: updateInChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
