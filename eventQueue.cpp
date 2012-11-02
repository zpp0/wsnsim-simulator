/**
 *
 * File: eventQueue.cpp
 * Description: Simulator Event Queue class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "eventQueue.h"

#include "event.h"

// добавляем в очередь
void eventQueue::insert(Event* event)
{
    queue.insert(event->time, event);
}

// извлекаем из очереди
Event* eventQueue::pop()
{
    if (queue.isEmpty())
        return NULL;

    Event* first = *(queue.lowerBound(0));
    queue.remove(first->time, first);

    return first;
}
