/**
 *
 * File: luaHost.cpp
 * Description: lua functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <cassert>

#include "luaHost.h"

lua_State* LuaHost::m_lua = 0;
QString LuaHost::m_errorString;

void LuaHost::open()
{
    // getting new state
    m_lua = luaL_newstate();

    luaL_openlibs(m_lua);

    // creation wsnsim
    lua_newtable(m_lua);
    lua_pushliteral(m_lua, "modules");
    lua_newtable(m_lua);
    lua_settable(m_lua, 1);
    lua_setglobal(m_lua, "wsnsim");

    // QString luaModules = "package.path = package.path .. \";"
    //     + currentProjectPath + "/?.lua\"";

    // luaL_dostring(L, luaModules.toUtf8().constData());

    // luaL_dostring(L, "jit.opt.start(3)");

    // return m_lua;

}

int LuaHost::loadFile(QString path)
{
    int ret = luaL_dofile(m_lua, path.toUtf8().constData());
    if (ret)
        m_errorString = lua_tostring(m_lua, -1);
    return ret;
}

int LuaHost::createModule(quint16 ID, QString name, ModuleID moduleID)
{
    const char* moduleName = name.toUtf8().constData();

    // -- put wsnsim.modules on top
    lua_getglobal(m_lua, "wsnsim");
    assert(lua_istable(m_lua, -1));

    lua_getfield(m_lua, -1, "modules");
    assert(lua_istable(m_lua, -1));

    // --- getting table for module
    lua_rawgeti(m_lua, -1, moduleID);
    if (!lua_istable(m_lua, -1)) {

        // table not found, create a new one

        lua_pushinteger(m_lua, moduleID);
        lua_newtable(m_lua);
        lua_settable(m_lua, 2);

        lua_pop(m_lua, 1);
        lua_rawgeti(m_lua, -1, moduleID);
    }

    // --- get new instance of module with ID
    lua_pushinteger(m_lua, ID);

    lua_getglobal(m_lua, moduleName);
    assert(lua_istable(m_lua, -1));

    lua_getfield(m_lua, -1, "new");
    if (!lua_isfunction(m_lua, -1)) {
        m_errorString = "module " + name + " has no new() method";
        return 0;
    }

    lua_getglobal(m_lua, moduleName);

    if (lua_pcall(m_lua, 1, 1, 0)) {
        m_errorString = lua_tostring(m_lua, -1);
        return 0;
    }

    lua_remove(m_lua, -2);
    lua_settable(m_lua, -3);

    return 1;
}

void LuaHost::close()
{
    lua_close(m_lua);
}

QString LuaHost::errorString()
{
    return m_errorString;
}
