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
    // извлечь из очереди очередное событие
    event* pop();
    // вставляет событие в нужное по хранологии место в очереди
    void insert(event* event);
    
private:
    // очередь событий
    QMultiMap<virtualTime, event*> queue;
};

#endif // EVENTQUEUE_H
