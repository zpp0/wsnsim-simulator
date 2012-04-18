/**
 *
 * File: event.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef EVENT_H
#define EVENT_H

#include "types.h"

/**
 *
 * виртуальный класс отвечает за все события в системе
 * является родительским для дерева классов событий
 *
 **/
class event
{
public:
    event();
    virtual ~event(){};

    static quint64 count;
    
    // момент времени, в который должно произойти событие (мкс)
    VirtualTime time;

    // FIXME: убрать это отсюда
    bool recordable;

    virtual QString eventName() const = 0;

    virtual void process() = 0;

    virtual void record(QDataStream& stream) = 0;

};

#endif // EVENT_H
