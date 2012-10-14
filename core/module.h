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
typedef QMap<QString, QPair<ModuleType, ModuleID> > ModuleDependencies;

struct Module
{
    QString name;
    ModuleID ID;
    ModuleType type;
    QString fileName;
    QVariantMap params;
    ModuleDependencies dependencies;
};

#endif // MODULE_H
