/****************************************************************************
** Meta object code from reading C++ file 'main.hh'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "main.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetSocket[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_NetSocket[] = {
    "NetSocket\0"
};

void NetSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData NetSocket::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NetSocket::staticMetaObject = {
    { &QUdpSocket::staticMetaObject, qt_meta_stringdata_NetSocket,
      qt_meta_data_NetSocket, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetSocket))
        return static_cast<void*>(const_cast< NetSocket*>(this));
    return QUdpSocket::qt_metacast(_clname);
}

int NetSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QUdpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ChatDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      31,   11,   11,   11, 0x0a,
      68,   57,   11,   11, 0x0a,
     119,  102,   11,   11, 0x0a,
     189,  165,   11,   11, 0x0a,
     231,  224,   11,   11, 0x0a,
     251,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ChatDialog[] = {
    "ChatDialog\0\0gotReturnPressed()\0"
    "processPendingDatagrams()\0inMap,port\0"
    "processRumor(QVariantMap,quint16)\0"
    "neighborMap,port\0"
    "processStatus(QMap<QString,QVariant>,quint16)\0"
    "myOrigin,mySeqNo,myPort\0"
    "sendRumor(QString,QString,quint16)\0"
    "myPort\0sendStatus(quint16)\0"
    "processAntiEntropy()\0"
};

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ChatDialog *_t = static_cast<ChatDialog *>(_o);
        switch (_id) {
        case 0: _t->gotReturnPressed(); break;
        case 1: _t->processPendingDatagrams(); break;
        case 2: _t->processRumor((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 3: _t->processStatus((*reinterpret_cast< QMap<QString,QVariant>(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 4: _t->sendRumor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 5: _t->sendStatus((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 6: _t->processAntiEntropy(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ChatDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ChatDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ChatDialog,
      qt_meta_data_ChatDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChatDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChatDialog))
        return static_cast<void*>(const_cast< ChatDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
