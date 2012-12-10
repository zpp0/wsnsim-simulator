/**
 *
 * File: module.h
 * Description: modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QList>
#include <QVariant>

#include "projectParams.h"
#include "event.h"

enum ModuleType {
    ModuleType_Undefined,
    ModuleType_Environment,
    ModuleType_Hardware,
    ModuleType_Software,
};

typedef quint16 ModuleID;
typedef int ModuleInstanceID;

enum ModuleParamType {
    ModuleParamType_Undefined,
    ModuleParamType_BOOL,
    ModuleParamType_INT,
    ModuleParamType_UINT8,
    ModuleParamType_UINT16,
    ModuleParamType_UINT32,
    ModuleParamType_UINT64,
    ModuleParamType_DOUBLE,
    ModuleParamType_STRING,
    ModuleParamType_NODES,
    ModuleParamType_TABLE
};

struct ModuleParameter
{
    QString name;
    ModuleParamType type;
    QVariant value;
};

struct ModuleDepend
{
    QString name;
    ModuleType type;
    ModuleID moduleID;
    bool hasFunctions;
    QMap<QString, QVector<EventParam> > eventParams;
};

struct Module
{
    Module(ModuleData& moduleData);

    QString name;
    ModuleID ID;
    ModuleType type;
    QString fileName;
    QList<ModuleParameter> params;
    QList<ModuleDepend> dependencies;
};

#endif // MODULE_H
