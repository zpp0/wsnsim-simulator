/**
 *
 * File: systemEventHandler.cpp
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#include "systemEventHandler.h"

#include "luaHost.h"

SystemEventHandler::SystemEventHandler(ModuleID moduleID, ModuleInstanceID ID, QString name)
{
    m_moduleID = moduleID;
    m_ID = ID;
    m_name = name;
}

void SystemEventHandler::handle(QVector<EventParam>& params)
{
    LuaHost::systemEventHandler(m_name, m_moduleID, m_ID, params);
}
