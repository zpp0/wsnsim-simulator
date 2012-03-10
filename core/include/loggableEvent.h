/**
 *
 * File: loggableEvent.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef LOGGABLEEVENT_H
#define LOGGABLEEVENT_H

#include <QString>

// #include "event.h"
#include "types.h"

class loggableEvent
{
public:
    virtual QString eventName() const = 0;

    virtual void record(QDataStream& stream) = 0;
};

#endif // LOGGABLEEVENT_H
