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
    virtual ~nodeEvent(){};

    //ссылка на узел, с которым происходит событие
    NodeID eventNode;
};

#endif // NODEEVENT_H
