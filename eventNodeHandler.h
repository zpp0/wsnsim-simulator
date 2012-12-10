/**
 *
 * File: eventNodeHandler.h
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENTNODEHANDLER_H
#define EVENTNODEHANDLER_H

#include "IHandler.h"
#include "nodes.h"

class EventNodeHandler : protected IHandler
{
public:
    EventNodeHandler(ModuleID moduleID,
                     ModuleInstanceID ID,
                     NodeID nodeID);

    void handle(Event* event);

private:
    NodeID m_nodeID;
};


#endif // EVENTNODEHANDLER_H
