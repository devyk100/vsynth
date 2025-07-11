/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/vsynth/MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[38];
    char stringdata0[11];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[15];
    char stringdata5[17];
    char stringdata6[17];
    char stringdata7[18];
    char stringdata8[6];
    char stringdata9[25];
    char stringdata10[6];
    char stringdata11[21];
    char stringdata12[22];
    char stringdata13[16];
    char stringdata14[15];
    char stringdata15[16];
    char stringdata16[14];
    char stringdata17[16];
    char stringdata18[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 15),  // "onAttackChanged"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 5),  // "value"
        QT_MOC_LITERAL(34, 14),  // "onDecayChanged"
        QT_MOC_LITERAL(49, 16),  // "onSustainChanged"
        QT_MOC_LITERAL(66, 16),  // "onReleaseChanged"
        QT_MOC_LITERAL(83, 17),  // "onWaveformChanged"
        QT_MOC_LITERAL(101, 5),  // "index"
        QT_MOC_LITERAL(107, 24),  // "onOscillatorCountChanged"
        QT_MOC_LITERAL(132, 5),  // "count"
        QT_MOC_LITERAL(138, 20),  // "onVibratoRateChanged"
        QT_MOC_LITERAL(159, 21),  // "onVibratoDepthChanged"
        QT_MOC_LITERAL(181, 15),  // "onReverbChanged"
        QT_MOC_LITERAL(197, 14),  // "onDelayChanged"
        QT_MOC_LITERAL(212, 15),  // "onRecordToggled"
        QT_MOC_LITERAL(228, 13),  // "onPlayToggled"
        QT_MOC_LITERAL(242, 15),  // "onExportClicked"
        QT_MOC_LITERAL(258, 16)   // "updateFFTDisplay"
    },
    "MainWindow",
    "onAttackChanged",
    "",
    "value",
    "onDecayChanged",
    "onSustainChanged",
    "onReleaseChanged",
    "onWaveformChanged",
    "index",
    "onOscillatorCountChanged",
    "count",
    "onVibratoRateChanged",
    "onVibratoDepthChanged",
    "onReverbChanged",
    "onDelayChanged",
    "onRecordToggled",
    "onPlayToggled",
    "onExportClicked",
    "updateFFTDisplay"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x08,    1 /* Private */,
       4,    1,  101,    2, 0x08,    3 /* Private */,
       5,    1,  104,    2, 0x08,    5 /* Private */,
       6,    1,  107,    2, 0x08,    7 /* Private */,
       7,    1,  110,    2, 0x08,    9 /* Private */,
       9,    1,  113,    2, 0x08,   11 /* Private */,
      11,    1,  116,    2, 0x08,   13 /* Private */,
      12,    1,  119,    2, 0x08,   15 /* Private */,
      13,    1,  122,    2, 0x08,   17 /* Private */,
      14,    1,  125,    2, 0x08,   19 /* Private */,
      15,    0,  128,    2, 0x08,   21 /* Private */,
      16,    0,  129,    2, 0x08,   22 /* Private */,
      17,    0,  130,    2, 0x08,   23 /* Private */,
      18,    0,  131,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onAttackChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDecayChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSustainChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onReleaseChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onWaveformChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onOscillatorCountChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onVibratoRateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onVibratoDepthChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onReverbChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDelayChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onRecordToggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPlayToggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFFTDisplay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAttackChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->onDecayChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onSustainChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->onReleaseChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onWaveformChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onOscillatorCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onVibratoRateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onVibratoDepthChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->onReverbChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->onDelayChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->onRecordToggled(); break;
        case 11: _t->onPlayToggled(); break;
        case 12: _t->onExportClicked(); break;
        case 13: _t->updateFFTDisplay(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
