/**
 *
 * File: eventQueue.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "eventQueue.h"

#include "event.h"

// добавляем в очередь
// void eventQueue::insert(processableEvent* event)
void eventQueue::insert(event* event)
{
    queue.insert(event->time, event);
}

// извлекаем из очереди
// processableEvent* eventQueue::pop()
event* eventQueue::pop()
{
    if (queue.isEmpty())
        return NULL;

    event* first = *(queue.lowerBound(0));
    // processableEvent* first = *(queue.lowerBound(0));
    // loggableEvent* f = (loggableEvent*) first;
    queue.remove(first->time, first);

    return first;
}
