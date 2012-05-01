/**
 *
 * File: types.h
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
typedef void(*InterruptHandler)();

enum ModuleType {
    ModuleType_Environment,
    ModuleType_Hardware,
    ModuleType_Software
};

struct ModuleInfo
{
    QString name;
    QString version;
    QString description;
    // module type: Environment, Hardware or Software
    // FIXME: is it real necessary?
    ModuleType type;
    // name of exporting interfaces
    QList<QString> exportInterfaces;
    // name of importing interfaces
    QList<QString> importInterfaces;
    // name and types of input params
    QMap<QString, QString> params;
};

// struct InterfaceInfo
// {
//     QString name;
// };

#endif // TYPES_H
