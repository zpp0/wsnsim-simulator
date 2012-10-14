/**
 *
 * File: event.h
 * Description: Simulator Event class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "module.h"

struct Event
{
    QString name;
    Module* author;
    // TODO: move to core
    // bool priority;
    // TODO: move to core
    bool recordable;
    VirtualTime time;
    // WARNING: in params can be written anything
    QVariantList params;
};

struct EventFactory
{
    void post(Module* author, QString name, VirtualTime time, QVariantList params);
};

#endif // EVENT_H
