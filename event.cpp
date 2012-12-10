/**
 *
 * File: event.h
 * Description: Simulator Event class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "event.h"

EventParamType getEventType(QString typeName)
{
    EventParamType type;

    if (typeName == "int32")
        type = INT32_TYPE;
    else if (typeName == "uint8")
        type = UINT8_TYPE;
    else if (typeName == "uint16")
        type = UINT16_TYPE;
    else if (typeName == "uint32")
        type = UINT32_TYPE;
    else if (typeName == "uint64")
        type = UINT64_TYPE;
    else if (typeName == "double")
        type = DOUBLE_TYPE;
    else if (typeName == "string")
        type = STRING_TYPE;
    else if (typeName == "ByteArray")
        type = BYTE_ARRAY_TYPE;
    else if (typeName == "bool")
        type = BOOL_TYPE;
    else
        type = UNKNOWN_TYPE;

    return type;
}

EventParam::EventParam(EventArgument param)
{
    name = param["name"];
    ID = param["ID"].toUInt();
    QString typeName = param["type"];

    type = getEventType(typeName);
}

Event::Event(EventParams event)
{
    name = event.eventInfo["name"];
    author = event.eventInfo["moduleID"].toUInt();
    ID = event.eventInfo["ID"].toUInt();

    recordable = true;
    QString recordableString = event.eventInfo["recordable"];
    if (recordableString == "false")
        recordable = false;

    foreach (EventArgument arg, event.arguments)
        params += EventParam(arg);
}
