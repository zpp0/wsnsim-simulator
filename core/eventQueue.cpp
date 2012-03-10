/**
 *
 * File: eventQueue.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "eventQueue.h"

//добавляем в очередь
void eventQueue::insert(event* event)
{
    queue.insert(event->time, event);
}

// извлекаем из очереди
event* eventQueue::pop()
{
    if (queue.isEmpty())
        return NULL;

    event* first = *(queue.lowerBound(0));
    queue.remove(first->time, first);

    return first;
}
