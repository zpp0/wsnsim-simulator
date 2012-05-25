/**
 *
 * File: types.h
 * Description: types declarations
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef TYPES_H
#define TYPES_H

#include <QtCore>

typedef quint64 VirtualTime;
typedef quint16 NodeID;
typedef quint8 EventID;
typedef QByteArray byteArray;

struct ModuleInfo
{
    QString name;
    QString version;
    QString description;
    // name of exporting interface
    QString exportInterface;
    // name of importing interfaces
    QList<QString> importInterfaces;
    // name and types of input params
    QMap<QString, QString> params;
    QMap<QString, QString> paramDescription;
    QList<QString> handledEvents;
};

enum InterfaceType {
    InterfaceType_Undefined,
    InterfaceType_Core,
    InterfaceType_Environment,
    InterfaceType_Hardware,
    InterfaceType_Software,
};

struct InterfaceInfo
{
    QString name;
    // interface type: Core, Environment, Hardware or Software
    InterfaceType type;

    // events
    // TODO: do eventParamType as enum
    // (eventName, (eventParam, eventParamType))
    QMap<QString, QList<QPair<QString, QString> > > events;
    QList<QString> proirityEvents;
    // (eventName, groupName)
    QMap<QString, QString> eventGroups;
    QMap<QString, QString> eventDescription;
};

#endif // TYPES_H
