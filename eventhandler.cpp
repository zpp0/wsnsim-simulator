/**
 *
 * File: luaEventHandler.cpp
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "eventHandler.h"
#include "luaHost.h"

EventHandler::EventHandler(ModuleID moduleID, ModuleInstanceID ID)
{
    m_moduleID = moduleID;
    m_ID = ID;
}

void EventHandler::handle(Event* event)
{
    LuaHost::eventHandler(event, m_moduleID, m_ID);
}

