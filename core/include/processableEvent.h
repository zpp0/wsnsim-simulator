/**
 *
 * File: processableEvent.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef PROCESSABLEEVENT_H
#define PROCESSABLEEVENT_H

#include "event.h"

/**
 *
 * класс обрабатываемых событий
 *
 **/
class processableEvent : public event
{
public:
    // основной метод класса. Реализует логику, которая выполняется в момент наступления события.
    virtual void process() = 0;
};

#endif // PROCESSABLEEVENT_H
