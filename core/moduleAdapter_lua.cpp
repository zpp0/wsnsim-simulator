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
    if (ret)
        m_errorString = LuaHost::errorString();
    return ret;
}

ModuleInstanceID ModuleAdapterLua::create()
{
    // Module* module = new Module();
    // m_ID++;

    // std::cout << "after creation on stack " << lua_gettop(m_lua) << std::endl;

    return m_ID++;
}

void ModuleAdapterLua::init(ModuleInstanceID moduleInstance)
{
}

QString ModuleAdapterLua::errorString()
{
    return m_errorString;
}
