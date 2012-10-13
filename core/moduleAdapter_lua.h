/**
 *
 * File: moduleAdapter_lua.h
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULEADAPTERLUA_H
#define MODULEADAPTERLUA_H

#include "moduleAdapter.h"

class ModuleAdapterLua : protected ModuleAdapter
{
public:
    ModuleAdapterLua(ModuleInitData moduleInitData);

    /* virtual */ int load();
    /* virtual */ Module* create();
    /* virtual */ void init(Module* module, QList<Module*> dependencies);
    /* virtual */ QString errorString();

private:
    QString m_errorString;
};

#endif // MODULEADAPTERLUA_H


