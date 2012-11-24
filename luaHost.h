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
#include "event.h"

class LuaHost
{
public:
    static void open();

    static int loadFile(QString path, QString name);
    static void removeGlobalName(QString name);

    static int createModule(ModuleID moduleID, ModuleInstanceID ID, QString name);

    static int initModule(ModuleID moduleID,
                          ModuleInstanceID ID,
                          ModuleType type,
                          QList<ModuleParameter> params,
                          QList<ModuleDepend> dependencies);

    static void close();

    static void eventHandler(Event* event,
                             ModuleID moduleID,
                             ModuleInstanceID ID);

    static QString errorString();

private:
    inline static void getInstance(ModuleID moduleID, ModuleInstanceID ID);

    static void createParams(QList<ModuleParameter> params);
    static void createDependencies(ModuleInstanceID ID,
                                   ModuleType type,
                                   QList<ModuleDepend> dependencies);

    static int handleEvent(lua_State* lua);
    static int postEvent(lua_State* lua);
    static int getTime(lua_State* lua);
    static int dummyDeclareModule(lua_State* lua);

    static void setCurrentModule(ModuleID moduleID, ModuleInstanceID ID);

    static lua_State *m_lua;
    static QString m_errorString;

    static ModuleID m_currentModule;
    static ModuleInstanceID m_currentModuleInstance;

    static QMap<ModuleID, ModuleInstanceID> m_modulesInstances;
    static QMap<ModuleID, QList<ModuleID> > m_moduleDeps;

    static QList<ModuleID> m_nodesModules;

    static QMap<ModuleID, QMap<ModuleInstanceID, int> > m_modulesRefs;
    static QMap<ModuleID, QMap<ModuleInstanceID, QMap<EventID, int> > > m_handlersRefs;
    static QMap<const void*, ModuleID> m_modulesPtrs;
    static QMap<const void*, ModuleInstanceID> m_modulesInstancesPtrs;
};

#endif // LUAHOST_H
