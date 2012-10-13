/**
 *
 * File: moduleAdapter_lua.cpp
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "moduleAdapter_lua.h"
#include "luaHost.h"

int ModuleAdapterLua::load(Qstring fileName)
{
    int ret = luaHost::load(fileName);
    if (ret)
        m_errorString = luaHost::errorString();
    return ret;
}

Module* ModuleAdapterLua::create()
{
    return NULL;
}

void ModuleAdapterLua::init(Module* module, QMap<QString, QVariant> params, QList<Module*> dependencies)
{
}
