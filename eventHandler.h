/**
 *
 * File: eventHandler.h
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "IHandler.h"

class EventHandler : protected IHandler
{
public:
    EventHandler(ModuleID moduleID, ModuleInstanceID ID);

    void handle(Event* event);
};

#endif // EVENTHANDLER_H
