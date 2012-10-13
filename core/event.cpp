/**
 *
 * File: event.cpp
 * Description: Simulator Environment class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "event.h"

#include "env.h"
#include "simulator.h"

void EventFactory::post(Module* author, QString name, VirtualTime time, QVariantList params)
{
    // TODO: bufferization
    Event* event = new Event();
    event->name = name;
    event->author = author;
    event->time = Env::time + time;
    if (time == 0)
        event->recordable = false;
    else
        event->recordable = true;
    event->params = params;

    Simulator::postEvent(author, event);
};
