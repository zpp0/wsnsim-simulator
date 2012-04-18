/**
 *
 * File: loggableEvent.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef LOGGABLEEVENT_H
#define LOGGABLEEVENT_H

#include <QString>

#include "event.h"
#include "types.h"

/**
 *
 * класс журналируемых событий
 *
 **/
class loggableEvent : public event
{
public:
    // момент времени, в который должно произойти событие (мкс)
    // VirtualTime time;

    virtual QString eventName() const = 0;

    virtual void record(QDataStream& stream) = 0;
};

#endif // LOGGABLEEVENT_H
