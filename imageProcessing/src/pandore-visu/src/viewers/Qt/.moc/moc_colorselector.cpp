/****************************************************************************
** Meta object code from reading C++ file 'colorselector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../colorselector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'colorselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ColorSelector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      26,   23,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   14,   14,   14, 0x08,
      61,   54,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ColorSelector[] = {
    "ColorSelector\0\0reset()\0id\0newLut(int)\0"
    "resetSettings()\0button\0"
    "selectLut(QAbstractButton*)\0"
};

void ColorSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ColorSelector *_t = static_cast<ColorSelector *>(_o);
        switch (_id) {
        case 0: _t->reset(); break;
        case 1: _t->newLut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->resetSettings(); break;
        case 3: _t->selectLut((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ColorSelector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ColorSelector::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ColorSelector,
      qt_meta_data_ColorSelector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ColorSelector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ColorSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ColorSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColorSelector))
        return static_cast<void*>(const_cast< ColorSelector*>(this));
    if (!strcmp(_clname, "Controller"))
        return static_cast< Controller*>(const_cast< ColorSelector*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ColorSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ColorSelector::reset()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ColorSelector::newLut(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
