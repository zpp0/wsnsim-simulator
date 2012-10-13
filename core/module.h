/**
 *
 * File: module.h
 * Description: modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
b *
 **/

#ifndef MODULES_H
#define MODULES_H

#include <QtCore>

typedef quint16 ModuleID;

enum ModuleType {
    ModuleType_Undefined,
    ModuleType_Environment,
    ModuleType_Hardware,
    ModuleType_Software,
};

struct ModuleInitData
{
    QString name;
    ModuleID ID;
    ModuleType type;
    QString fileName;
    QVariantMap params;
    QList<quint16> dependencies;
};

struct Module
{
    QString name;
    ModuleID ID;
    ModuleType type;
    QStringList paramsNames;
    QStringList interfaceFunctions;
    QStringList events;
    QMap<QString, void*> eventHandlers;
};

#endif // MODULES_H
