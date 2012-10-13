/**
 *
 * File: module.h
 * Description: modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULES_H
#define MODULES_H

#include <QtCore>

enum ModuleType {
    ModuleType_Undefined,
    ModuleType_Environment,
    ModuleType_Hardware,
    ModuleType_Software,
};

struct Module
{
    QString name;
    quint16 ID;
    ModuleType type;
    QStringList paramsNames;
    QStringList interfaceFunctions;
    QStringList events;
    QList<quint16> dependencies;
    QMap<QString, void*> eventHandlers;
};

class ModuleAdapter {
public:
    virtual void load(QString fileName) = 0;
    virtual Module* create() = 0;
    virtual void init(Module* module, QMap<QString, QVariant> params, QList<Module*> dependencies) = 0;
    virtual QString errorString() = 0;
};

#endif // MODULES_H
