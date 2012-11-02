/**
 *
 * File: luaEventHandler.h
 * Description: Event Handler wrapper on lua
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QtCore>

#include "event.h"

class EventHandler
{
public:
    EventHandler(ModuleID moduleID, ModuleInstanceID ID);
    void handle(Event* event);

private:
    ModuleID m_moduleID;
    ModuleInstanceID m_ID;
};

#endif // EVENTHANDLER_H
