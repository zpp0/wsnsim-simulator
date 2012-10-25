/**
 *
 * File: luaEventHandler.h
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef LUAEVENTHANDLER_H
#define LUAEVENTHANDLER_H

#include <QtCore>

#include "event.h"

class LuaEventHandler
{
public:
    LuaEventHandler(ModuleID moduleID, ModuleInstanceID ID, QString handlerName);
    void handle(Event* event);
private:
    ModuleID m_moduleID;
    ModuleInstanceID m_ID;
    QString m_handlerName;
};

#endif // LUAEVENTHANDLER_H
