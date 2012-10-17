/**
 *
 * File: simulator_api.cpp
 * Description: Simulator API
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "simulator_api.h"
#include "simulator.h"
#include "event.h"

VirtualTime SimulatorAPI::globalTime()
{
    return Simulator::globalTime();
}

void SimulatorAPI::postEvent(ModuleID author, QString name, VirtualTime time, QVariantList params)
{
    // TODO: bufferization
    Event* event = new Event();
    event->name = name;
    event->author = author;
    event->time = Simulator::globalTime() + time;
    if (time == 0)
        event->recordable = false;
    else
        event->recordable = true;
    event->params = params;

    Simulator::postEvent(author, event);
};

