/**
 *
 * File: event.h
 * Description: Simulator Event class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "module.h"

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

struct EventParam
{
    quint8 ID;
    QString name;
    EventParamType type;
    QVariant value;
};

struct Event
{
    EventID ID;
    QString name;
    ModuleID author;
    ModuleInstanceID authorID;
    bool recordable;
    VirtualTime time;
    QList<EventParam> params;
};

#endif // EVENT_H
