/****************************************************************************
** Meta object code from reading C++ file 'MeshViewerWidget.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MeshViewerWidget.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeshViewerWidget.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MeshViewerWidget_t {
    QByteArrayData data[7];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MeshViewerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MeshViewerWidget_t qt_meta_stringdata_MeshViewerWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MeshViewerWidget"
QT_MOC_LITERAL(1, 17, 20), // "query_open_mesh_file"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 23), // "query_open_texture_file"
QT_MOC_LITERAL(4, 63, 22), // "query_open_scalar_file"
QT_MOC_LITERAL(5, 86, 19), // "query_get_threshold"
QT_MOC_LITERAL(6, 106, 15) // "query_animation"

    },
    "MeshViewerWidget\0query_open_mesh_file\0"
    "\0query_open_texture_file\0"
    "query_open_scalar_file\0query_get_threshold\0"
    "query_animation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeshViewerWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MeshViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MeshViewerWidget *_t = static_cast<MeshViewerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->query_open_mesh_file(); break;
        case 1: _t->query_open_texture_file(); break;
        case 2: _t->query_open_scalar_file(); break;
        case 3: _t->query_get_threshold(); break;
        case 4: _t->query_animation(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MeshViewerWidget::staticMetaObject = {
    { &MeshViewerWidgetT<MyMesh>::staticMetaObject, qt_meta_stringdata_MeshViewerWidget.data,
      qt_meta_data_MeshViewerWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MeshViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeshViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MeshViewerWidget.stringdata0))
        return static_cast<void*>(this);
    return MeshViewerWidgetT<MyMesh>::qt_metacast(_clname);
}

int MeshViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MeshViewerWidgetT<MyMesh>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
