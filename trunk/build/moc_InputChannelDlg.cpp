/****************************************************************************
** Meta object code from reading C++ file 'InputChannelDlg.h'
**
** Created: Wed Sep 24 11:54:20 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/include/InputChannelDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputChannelDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InputChannelDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   16,   16,   16, 0x0a,
      50,   16,   16,   16, 0x0a,
      59,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_InputChannelDlg[] = {
    "InputChannelDlg\0\0,\0updateInChn(int,int*)\0"
    "accept()\0reject()\0appear()\0"
};

const QMetaObject InputChannelDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_InputChannelDlg,
      qt_meta_data_InputChannelDlg, 0 }
};

const QMetaObject *InputChannelDlg::metaObject() const
{
    return &staticMetaObject;
}

void *InputChannelDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InputChannelDlg))
        return static_cast<void*>(const_cast< InputChannelDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int InputChannelDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateInChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 1: accept(); break;
        case 2: reject(); break;
        case 3: appear(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void InputChannelDlg::updateInChn(int _t1, int * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
