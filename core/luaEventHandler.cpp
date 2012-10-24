/**
 *
 * File: luaEventHandler.cpp
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "luaEventHandler.h"
#include "luaHost.h"

void LuaEventHandler::handle(Event* event)
{
    LuaHost::eventHandler(event);
}

