/**
 *
 * File: event.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef EVENT_H
#define EVENT_H

#include "types.h"

struct Event
{
    QString name;
    // TODO: move to core
    // bool priority;
    // TODO: move to core
    // bool recordable;
    VirtualTime time;
    // WARNING: in params can be written anything
    QVariantList params;
};

#include "IEvent.h"

#include "IModule.h"

struct EventFactory : public IEvent
{
    /* virtual */ void post(IModule* author, QString name, VirtualTime time, QVariantList params);
};

#endif // EVENT_H
