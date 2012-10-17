/**
 *
 * File: luaHost.h
 * Description: lua functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef LUAHOST_H
#define LUAHOST_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <QString>

#include "module.h"
#include "types.h"
#include "event.h"

class LuaHost
{
public:
    static void open();

    static int loadFile(QString path);

    static int createModule(ModuleInstanceID ID, QString name, ModuleID moduleID);

    static int initModule(ModuleID moduleID,
                          ModuleInstanceID ID,
                          ModuleType type,
                          QList<ModuleParameter> params,
                          QList<ModuleDependence> dependencies);

    static void close();

    static QString errorString();

private:
    static void getModulesTable();
    static void getModule(ModuleID moduleID);
    static void getInstance(ModuleInstanceID ID);
    static int createModule(const char* moduleName, ModuleInstanceID ID);

    static void createParams(QList<ModuleParameter> params);
    static void createDependencies(ModuleInstanceID ID,
                                   ModuleType type,
                                   QList<ModuleDependence> dependencies);

    static void eventHandler(Event* event);

    static lua_State *m_lua;
    static QString m_errorString;

    static QMap<ModuleID, ModuleInstanceID> m_modulesInstances;
};

#endif // LUAHOST_H
