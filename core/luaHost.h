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

    static void eventHandler(Event* event,
                             ModuleID moduleID,
                             ModuleInstanceID ID,
                             const char* handlerName);

    static QString errorString();

private:
    static void getModulesTable();
    inline static void getModule(ModuleID moduleID);
    inline static void getInstance(ModuleInstanceID ID);
    static int createModule(QString moduleName, ModuleInstanceID ID);

    static void createParams(QList<ModuleParameter> params);
    static void createDependencies(ModuleInstanceID ID,
                                   ModuleType type,
                                   QList<ModuleDependence> dependencies);

    static int handleEvent(lua_State* lua);
    static int postEvent(lua_State* lua);

    static void setCurrentModule(ModuleID moduleID, ModuleInstanceID ID);

    static lua_State *m_lua;
    static QString m_errorString;

    static ModuleID m_currentModule;
    static ModuleInstanceID m_currentModuleInstance;

    static QMap<ModuleID, ModuleInstanceID> m_modulesInstances;
    static QMap<ModuleID, QList<ModuleID> > m_moduleDeps;
};

#endif // LUAHOST_H
