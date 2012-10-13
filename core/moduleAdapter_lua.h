/**
 *
 * File: moduleAdapter_lua.h
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULEADAPTERLUA_H
#define MODULEADAPTERLUA_H

#include "modules.h"

class ModuleAdapterLua : public ModuleAdapter
{
public:
    /* virtual */ void load(Qstring  fileName);
    /* virtual */ Module* create();
    /* virtual */ void init(Module* module, QMap<QString, QVariant> params, QList<Module*> dependencies);
    /* virtual */ QString errorString();
private:
    QString m_errorString;
};

#endif // MODULEADAPTERLUA_H
