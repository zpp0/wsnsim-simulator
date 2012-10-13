/**
 *
 * File: moduleAdapter_lua.cpp
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "moduleAdapter_lua.h"
#include "luaHost.h"

ModuleAdapterLua::ModuleAdapterLua(ModuleInitData moduleInitData)
    :ModuleAdapter(moduleInitData)
{
}

int ModuleAdapterLua::load()
{
    int ret = LuaHost::loadFile(m_moduleInitData.fileName);
    if (ret)
        m_errorString = LuaHost::errorString();
    return ret;
}

Module* ModuleAdapterLua::create()
{
    return NULL;
}

void ModuleAdapterLua::init(Module* module, QList<Module*> dependencies)
{
}

QString ModuleAdapterLua::errorString()
{
    return m_errorString;
}
