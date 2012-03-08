/**
 *
 * File: processableEvent.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef PROCESSABLEEVENT_H
#define PROCESSABLEEVENT_H

class processableEvent
{
public:
    // основной метод класса. Реализует логику, которая выполняется в момент наступления события.
    virtual void process() = 0;
};

#endif // PROCESSABLEEVENT_H
