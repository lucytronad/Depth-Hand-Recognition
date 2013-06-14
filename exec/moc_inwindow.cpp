/****************************************************************************
** Meta object code from reading C++ file 'inwindow.h'
**
** Created: Mon Jan 7 19:31:22 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../IN5X/src/inwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_INWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      27,    9,    9,    9, 0x0a,
      40,    9,    9,    9, 0x0a,
      59,    9,    9,    9, 0x0a,
      69,    9,    9,    9, 0x0a,
      85,    9,    9,    9, 0x0a,
     101,    9,    9,    9, 0x0a,
     119,    9,    9,    9, 0x0a,
     137,    9,    9,    9, 0x0a,
     155,    9,    9,    9, 0x0a,
     173,    9,    9,    9, 0x0a,
     191,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_INWindow[] = {
    "INWindow\0\0updateSeuil(int)\0openDialog()\0"
    "createDataDialog()\0setData()\0"
    "setHandStateO()\0setHandStateC()\0"
    "setFingerState0()\0setFingerState1()\0"
    "setFingerState2()\0setFingerState3()\0"
    "setFingerState4()\0setFingerState5()\0"
};

void INWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        INWindow *_t = static_cast<INWindow *>(_o);
        switch (_id) {
        case 0: _t->updateSeuil((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->openDialog(); break;
        case 2: _t->createDataDialog(); break;
        case 3: _t->setData(); break;
        case 4: _t->setHandStateO(); break;
        case 5: _t->setHandStateC(); break;
        case 6: _t->setFingerState0(); break;
        case 7: _t->setFingerState1(); break;
        case 8: _t->setFingerState2(); break;
        case 9: _t->setFingerState3(); break;
        case 10: _t->setFingerState4(); break;
        case 11: _t->setFingerState5(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData INWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject INWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_INWindow,
      qt_meta_data_INWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &INWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *INWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *INWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_INWindow))
        return static_cast<void*>(const_cast< INWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int INWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
