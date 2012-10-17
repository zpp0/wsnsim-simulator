/**
 *
 * File: moduleAdapter_lua.cpp
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "moduleAdapter_lua.h"
#include "luaHost.h"

ModuleAdapterLua::ModuleAdapterLua(Module module)
    :ModuleAdapter(module)
{
    m_ID = 0;
}

int ModuleAdapterLua::load()
{
    int ret = LuaHost::loadFile(m_module.fileName);
    if (ret) {
        m_errorString = LuaHost::errorString();
        return 0;
    }

    return 1;
}

ModuleInstanceID ModuleAdapterLua::create()
{
    int ret = LuaHost::createModule(m_ID, m_module.name, m_module.ID);

    if (ret == 0) {
        m_errorString = LuaHost::errorString();
        return -1;
    }

    return m_ID++;
}

int ModuleAdapterLua::init(ModuleInstanceID moduleInstance)
{

    int ret = LuaHost::initModule(m_module.ID,
                                  moduleInstance,
                                  m_module.type,
                                  m_module.params,
                                  m_module.dependencies);
    if (ret == 0) {
        m_errorString = LuaHost::errorString();
        return 0;
    }

    return 1;
}

QString ModuleAdapterLua::errorString()
{
    return m_errorString;
}
