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
    // извлечь из очереди очередное событие
    loggableEvent* pop();
    // вставляет событие в нужное по хранологии место в очереди
    void insert(loggableEvent* event);
    
private:
    // очередь событий
    QMultiMap<virtualTime, loggableEvent*> queue;
};

#endif // EVENTQUEUE_H
