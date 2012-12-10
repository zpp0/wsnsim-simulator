/**
 *
 * File: eventHandler.cpp
 * Description: Event Handler
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "eventHandler.h"
#include "luaHost.h"

EventHandler::EventHandler(ModuleID moduleID, ModuleInstanceID ID)
    :IHandler(moduleID, ID)
{
}

void EventHandler::handle(Event* event)
{
    if (withCustomParams) {
        setCustomValues(event->params);
        LuaHost::eventHandler(event, m_moduleID, m_ID, m_params);
    }
    else
        LuaHost::eventHandler(event, m_moduleID, m_ID, event->params);
}
