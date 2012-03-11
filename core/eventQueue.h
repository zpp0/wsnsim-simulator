/**
 *
 * File: eventQueue.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <QMultiMap>

#include "loggableEvent.h"
#include "types.h"

class eventQueue
{
public:
    // вставить событие в нужное по хронологии место в очереди
    void insert(loggableEvent* event);

    // извлечь из очереди очередное событие
    loggableEvent* pop();
    
private:
    // очередь событий
    QMultiMap<VirtualTime, loggableEvent*> queue;
};

#endif // EVENTQUEUE_H
