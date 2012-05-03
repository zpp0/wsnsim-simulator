/**
 *
 * File: eventQueue.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <QMultiMap>

#include "event.h"
#include "types.h"

class eventQueue
{
public:
    // вставить событие в нужное по хронологии место в очереди
    void insert(Event* event);

    // извлечь из очереди очередное событие
    Event* pop();
    
private:
    // очередь событий
    QMultiMap<VirtualTime, Event*> queue;
};

#endif // EVENTQUEUE_H
