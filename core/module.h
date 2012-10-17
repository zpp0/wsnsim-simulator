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
    QVariantMap params;
    QList<ModuleDependence> dependencies;
};

#endif // MODULE_H
