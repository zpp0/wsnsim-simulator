/**
 *
 * File: luaEventHandler.cpp
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "luaEventHandler.h"
#include "luaHost.h"

LuaEventHandler::LuaEventHandler(ModuleID moduleID, ModuleInstanceID ID, const char* handlerName)
{
    m_moduleID = moduleID;
    m_ID = ID;
    m_handlerName = handlerName;
}

void LuaEventHandler::handle(Event* event)
{
    LuaHost::eventHandler(event, m_moduleID, m_ID, m_handlerName);
}

