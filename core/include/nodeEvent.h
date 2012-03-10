/**
 *
 * File: nodeEvent.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef NODEEVENT_H
#define NODEEVENT_H

#include "event.h"
#include "types.h"

/**
 *
 * класс отвечает за все события, связанные с узлами
 *
 **/
class nodeEvent : public event
{
public:
// nodeEvent(virtualTime Time, nodeID EventNode) :event(Time), eventNodeID(EventNode) {};
// nodeEvent() :event() {};
    virtual ~nodeEvent(){};

    //ссылка на узел, с которым происходит событие
    nodeID eventNodeID;
};

#endif // NODEEVENT_H
