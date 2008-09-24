/****************************************************************************
** Meta object code from reading C++ file 'DCThread.h'
**
** Created: Thu Aug 21 17:34:28 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DCThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DCThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DCThread[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   10,    9,    9, 0x05,
      33,   10,    9,    9, 0x05,
      53,   50,    9,    9, 0x05,
      82,   50,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DCThread[] = {
    "DCThread\0\0message\0error(QString)\0"
    "message(QString)\0,,\0addPoint1(double,double,int)\0"
    "addPoint2(double,double,int)\0"
};

const QMetaObject DCThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DCThread,
      qt_meta_data_DCThread, 0 }
};

const QMetaObject *DCThread::metaObject() const
{
    return &staticMetaObject;
}

void *DCThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DCThread))
        return static_cast<void*>(const_cast< DCThread*>(this));
    return QThread::qt_metacast(_clname);
}

int DCThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: message((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: addPoint1((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: addPoint2((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DCThread::error(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DCThread::message(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DCThread::addPoint1(double _t1, double _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DCThread::addPoint2(double _t1, double _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
