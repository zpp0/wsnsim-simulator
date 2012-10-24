/**
 *
 * File: luaHost.cpp
 * Description: lua functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <cassert>

#include "luaHost.h"
#include "luaEventHandler.h"
#include "eventHandler.hpp"

lua_State* LuaHost::m_lua = 0;
QString LuaHost::m_errorString;

QMap<ModuleID, ModuleInstanceID> LuaHost::m_modulesInstances;

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

int LuaHost::createModule(ModuleInstanceID ID, QString name, ModuleID moduleID)
{
    getModulesTable();
    getModule(moduleID);

    int ret = createModule(name, ID);

    lua_settop(m_lua, 0);

    if (ret == 0)
        return 0;

    if (ID == 0)
        m_modulesInstances[moduleID] = 0;
    else
        m_modulesInstances[moduleID]++;

    return 1;
}

int LuaHost::initModule(ModuleID moduleID,
                        ModuleInstanceID ID,
                        ModuleType type,
                        QList<ModuleParameter> params,
                        QList<ModuleDependence> dependencies)
{
    // put on of the stack top module instance
    getModulesTable();
    getModule(moduleID);
    getInstance(ID);

    lua_getfield(m_lua, -1, "init");
    if (!lua_isfunction(m_lua, -1)) {
        m_errorString = "module with ID " + QString::number(moduleID) + " has no init() method";
        return 0;
    }

    getModulesTable();
    getModule(moduleID);
    getInstance(ID);
    lua_remove(m_lua, -2);
    lua_remove(m_lua, -2);
    lua_remove(m_lua, -2);

    // put on top of the stack module params
    createParams(params);

    // put on top of the stack module interfaces
    createDependencies(ID, type, dependencies);

    // call init function
    if (lua_pcall(m_lua, 3, 0, 0)) {
        m_errorString = lua_tostring(m_lua, -1);
        return 0;
    }

    lua_settop(m_lua, 0);
    return 1;
}

void LuaHost::getModulesTable()
{
    // -- put wsnsim.modules on top
    lua_getglobal(m_lua, "wsnsim");
    assert(lua_istable(m_lua, -1));

    lua_getfield(m_lua, 1, "modules");
    assert(lua_istable(m_lua, -1));
}

void LuaHost::getModule(ModuleID moduleID)
{
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
}

void LuaHost::getInstance(ModuleInstanceID ID)
{
    // --- getting table for module
    lua_rawgeti(m_lua, -1, ID);
    assert(lua_istable(m_lua, -1));

}

int LuaHost::createModule(QString moduleName, ModuleInstanceID ID)
{
    // --- get new instance of module with ID
    lua_pushinteger(m_lua, ID);

    lua_getglobal(m_lua, moduleName.toUtf8().constData());
    assert(lua_istable(m_lua, -1));

    lua_getfield(m_lua, -1, "new");
    if (!lua_isfunction(m_lua, -1)) {
        m_errorString = "module " + moduleName + " has no new() method";
        return 0;
    }

    lua_getglobal(m_lua, moduleName.toUtf8().constData());

    if (lua_pcall(m_lua, 1, 1, 0)) {
        m_errorString = lua_tostring(m_lua, -1);
        return 0;
    }

    lua_remove(m_lua, -2);
    lua_settable(m_lua, -3);

    return 1;
}

void LuaHost::createParams(QList<ModuleParameter> params)
{
    lua_newtable(m_lua);

    foreach(ModuleParameter param, params) {

        switch (param.type) {
        case ModuleParamType_INT:
            lua_pushnumber(m_lua, param.value.toInt());
            break;
        case ModuleParamType_UINT8:
        case ModuleParamType_UINT16:
        case ModuleParamType_UINT32:
        case ModuleParamType_UINT64:
            lua_pushnumber(m_lua, param.value.toUInt());
            break;
        case ModuleParamType_DOUBLE:
            lua_pushnumber(m_lua, param.value.toDouble());
            break;
        case ModuleParamType_STRING:
            lua_pushstring(m_lua, param.value.toString().toUtf8().constData());
            break;
        case ModuleParamType_Undefined:
            break;
        }

        lua_setfield(m_lua, -2, param.name.toUtf8().constData());
    }
}

void LuaHost::createDependencies(ModuleInstanceID ID,
                                 ModuleType type,
                                 QList<ModuleDependence> dependencies)
{
    lua_newtable(m_lua);

    foreach(ModuleDependence dep, dependencies) {

        switch(dep.type) {

        case ModuleType_Environment:
            if (type == ModuleType_Environment
                || type == ModuleType_Hardware) {
                getModulesTable();
                getModule(dep.moduleID);
                getInstance(0);
                lua_remove(m_lua, -2);
                lua_remove(m_lua, -2);
                lua_remove(m_lua, -2);
            }
            break;

        case ModuleType_Hardware:
        case ModuleType_Software:
            if (type == ModuleType_Hardware
                || ModuleType_Software) {
                getModulesTable();
                getModule(dep.moduleID);
                getInstance(ID);
                lua_remove(m_lua, -2);
                lua_remove(m_lua, -2);
            }

            else if(type == ModuleType_Environment) {
                for (ModuleInstanceID i = 0; i < m_modulesInstances[dep.moduleID]; i++) {
                    lua_newtable(m_lua);
                    lua_pushinteger(m_lua, i);

                    getModulesTable();
                    getModule(i);
                    lua_remove(m_lua, -2);

                    lua_settable(m_lua, -3);
                }
            }

            break;
        case ModuleType_Undefined:
            break;
        }

        lua_setfield(m_lua, -2, dep.name.toUtf8().constData());
    }
}

void LuaHost::close()
{
    lua_close(m_lua);
}

void LuaHost::eventHandler(Event* event)
{
    getModulesTable();
    getModule(event->author);
    getInstance(event->authorID);

    lua_getfield(m_lua, -1, event->name.toUtf8().constData());

    // TODO: errors handling
    // if (!lua_isfunction(m_lua, -1)) {
    //     return 0;
    // }

    getInstance(event->authorID);

    // FIXME: push the value with the true type
    foreach(QVariant param, event->params)
        lua_pushnumber(m_lua, param.toInt());

    // TODO: errors handling
    lua_pcall(m_lua, 1 + event->params.size(), 0, 0);

    lua_settop(m_lua, 0);
}

QString LuaHost::errorString()
{
    return m_errorString;
}
