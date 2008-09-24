/****************************************************************************
** Meta object code from reading C++ file 'mainwin.h'
**
** Created: Wed Sep 24 15:08:07 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/include/mainwin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyMainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      27,   25,   13,   13, 0x0a,
      51,   13,   13,   13, 0x08,
      69,   13,   13,   13, 0x08,
      88,   13,   13,   13, 0x08,
     107,   13,   13,   13, 0x08,
     126,   13,   13,   13, 0x08,
     145,   13,   13,   13, 0x08,
     164,   13,   13,   13, 0x08,
     183,   13,   13,   13, 0x08,
     201,   13,   13,   13, 0x08,
     219,   13,   13,   13, 0x08,
     237,   13,   13,   13, 0x08,
     255,   13,   13,   13, 0x08,
     273,   13,   13,   13, 0x08,
     291,   13,   13,   13, 0x08,
     309,   13,   13,   13, 0x08,
     326,   13,   13,   13, 0x08,
     350,   13,   13,   13, 0x08,
     363,   13,   13,   13, 0x08,
     376,   13,   13,   13, 0x08,
     389,   13,   13,   13, 0x08,
     401,   13,   13,   13, 0x08,
     412,   13,   13,   13, 0x08,
     427,   13,   13,   13, 0x08,
     442,   13,   13,   13, 0x08,
     455,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MyMainWindow[] = {
    "MyMainWindow\0\0DAQSetup()\0,\0"
    "updateSGInChn(int,int*)\0SGMethodChanged()\0"
    "Syn0ComboChanged()\0Syn1ComboChanged()\0"
    "Syn2ComboChanged()\0Syn3ComboChanged()\0"
    "Syn4ComboChanged()\0Syn5ComboChanged()\0"
    "HH0ComboChanged()\0HH1ComboChanged()\0"
    "HH2ComboChanged()\0HH3ComboChanged()\0"
    "HH4ComboChanged()\0HH5ComboChanged()\0"
    "StartButClicked()\0StopButClicked()\0"
    "DisplayMessage(QString)\0showDAQDlg()\0"
    "SaveConfig()\0LoadConfig()\0ExportLog()\0"
    "ClearLog()\0SaveProtocol()\0LoadProtocol()\0"
    "LoadScript()\0UnLoadScript()\0"
};

const QMetaObject MyMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MyMainWindow,
      qt_meta_data_MyMainWindow, 0 }
};

const QMetaObject *MyMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MyMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyMainWindow))
        return static_cast<void*>(const_cast< MyMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MyMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DAQSetup(); break;
        case 1: updateSGInChn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 2: SGMethodChanged(); break;
        case 3: Syn0ComboChanged(); break;
        case 4: Syn1ComboChanged(); break;
        case 5: Syn2ComboChanged(); break;
        case 6: Syn3ComboChanged(); break;
        case 7: Syn4ComboChanged(); break;
        case 8: Syn5ComboChanged(); break;
        case 9: HH0ComboChanged(); break;
        case 10: HH1ComboChanged(); break;
        case 11: HH2ComboChanged(); break;
        case 12: HH3ComboChanged(); break;
        case 13: HH4ComboChanged(); break;
        case 14: HH5ComboChanged(); break;
        case 15: StartButClicked(); break;
        case 16: StopButClicked(); break;
        case 17: DisplayMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: showDAQDlg(); break;
        case 19: SaveConfig(); break;
        case 20: LoadConfig(); break;
        case 21: ExportLog(); break;
        case 22: ClearLog(); break;
        case 23: SaveProtocol(); break;
        case 24: LoadProtocol(); break;
        case 25: LoadScript(); break;
        case 26: UnLoadScript(); break;
        }
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
