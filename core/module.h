/**
 *
 * File: module.h
 * Description: modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
b *
 **/

#ifndef MODULE_H
#define MODULE_H

#include <QtCore>

#include "types.h"

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
    ModuleParamType_INT,
    ModuleParamType_UINT8,
    ModuleParamType_UINT16,
    ModuleParamType_UINT32,
    ModuleParamType_UINT64,
    ModuleParamType_DOUBLE,
    ModuleParamType_STRING
};

struct ModuleParameter
{
    QString name;
    ModuleParamType type;
    QVariant value;
};

struct ModuleDependence
{
    QString name;
    ModuleType type;
    ModuleID moduleID;
};

struct Module
{
    QString name;
    ModuleID ID;
    ModuleType type;
    QString fileName;
    QList<ModuleParameter> params;
    QList<ModuleDependence> dependencies;
};

#endif // MODULE_H
