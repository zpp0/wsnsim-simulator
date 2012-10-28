/**
 *
 * File: luaEventHandler.cpp
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "luaEventHandler.h"
#include "luaHost.h"

LuaEventHandler::LuaEventHandler(ModuleID moduleID, ModuleInstanceID ID)
{
    m_moduleID = moduleID;
    m_ID = ID;
}

void LuaEventHandler::handle(Event* event)
{
    LuaHost::eventHandler(event, m_moduleID, m_ID);
}

