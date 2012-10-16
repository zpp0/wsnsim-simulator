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

class LuaHost
{
public:
    static void open();
    static int loadFile(QString path);
    static void close();

static int createModule(quint16 ID, QString name, ModuleID moduleID);

    static QString errorString();
private:
    static lua_State *m_lua;
    static QString m_errorString;
};

#endif // LUAHOST_H