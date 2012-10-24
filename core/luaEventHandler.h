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
    void handle(Event* event);
};

#endif // LUAEVENTHANDLER_H
