/**
 *
 * File: eventQueue.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <QMultiMap>

// #include "processableEvent.h"
#include "event.h"
#include "types.h"

class eventQueue
{
public:
    // вставить событие в нужное по хронологии место в очереди
    // void insert(processableEvent* event);
    void insert(event* event);

    // извлечь из очереди очередное событие
    // processableEvent* pop();
    event* pop();
    
private:
    // очередь событий
    // QMultiMap<VirtualTime, processableEvent*> queue;
    QMultiMap<VirtualTime, event*> queue;
};

#endif // EVENTQUEUE_H
