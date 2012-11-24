/**
 *
 * File: event.h
 * Description: Simulator Event class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENT_H
#define EVENT_H

#include "virtualTime.h"
#include "module.h"

typedef quint8 EventID;

enum EventParamType
{
    UNKNOWN_TYPE,
    UINT8_TYPE,
    UINT16_TYPE,
    UINT32_TYPE,
    UINT64_TYPE,
    INT32_TYPE,
    BOOL_TYPE,
    DOUBLE_TYPE,
    BYTE_ARRAY_TYPE,
    STRING_TYPE
};

union ParamValue {
    quint8 u8;
    quint16 u16;
    quint32 u32;
    quint64 u64;
    int i32;
    bool b;
    double d;
    struct byteArray {
        quint8 size;
        char* data;
    } byteArray;
    struct string {
        quint16 length;
        char* data;
    } string;
};

struct EventParam
{
    quint8 ID;
    QString name;
    EventParamType type;
    ParamValue value;
};

struct Event
{
    EventID ID;
    QString name;
    ModuleID author;
    bool recordable;
    VirtualTime time;
    QVector<EventParam> params;
};

#endif // EVENT_H
