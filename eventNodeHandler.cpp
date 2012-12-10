/**
 *
 * File: eventNodeHandler.cpp
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#include "eventNodeHandler.h"
#include "luaHost.h"

EventNodeHandler::EventNodeHandler(ModuleID moduleID,
                                   ModuleInstanceID ID,
                                   NodeID nodeID)
    : IHandler(moduleID, ID)
{
    m_nodeID = nodeID;
}

void EventNodeHandler::handle(Event* event)
{
    if (event->params[0].value.u16 == m_nodeID) {
        QVector<EventParam> params = event->params.mid(1);

        if (withCustomParams) {
            setCustomValues(params);
            LuaHost::eventHandler(event, m_moduleID, m_ID, m_params);
        }
        else
            LuaHost::eventHandler(event, m_moduleID, m_ID, params);
    }
}
