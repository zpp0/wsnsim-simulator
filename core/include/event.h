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
 * виртуальный класс отвечает за все события в системе
 * объекты этого класса хранятся в глобальной очереди объектов
 * является родительским для дерева классов событий
 **/
class event
{
public:
    // Конструктор принимает момент времени, в которой должно произойти событие.
    event(virtualTime Time) :time(Time) {}
    virtual ~event(){};
    
    // virtual bool check(){}

    // момент времени, в который должно произойти событие (мкс)
    virtualTime time;
};

#endif // EVENT_H
