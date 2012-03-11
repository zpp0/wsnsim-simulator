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
    virtual ~event(){};
    
    // момент времени, в который должно произойти событие (мкс)
    VirtualTime time;
};

#endif // EVENT_H
