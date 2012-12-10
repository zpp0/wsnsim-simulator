/**
 *
 * File: IHandler.cpp
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#include "IHandler.h"

IHandler::IHandler(ModuleID moduleID, ModuleInstanceID ID)
{
    m_moduleID = moduleID;
    m_ID = ID;
    withCustomParams = false;
}

void IHandler::setCustomParams(QVector<EventParam>& params)
{
    m_params = params;
    withCustomParams = true;
}

void IHandler::setCustomValues(QVector<EventParam>& params)
{
    for(int i = 0; i < m_params.size(); i++) {
        foreach(EventParam eventParam, params) {
            if (m_params[i].name == eventParam.name) {
                m_params[i].value = eventParam.value;
                break;
            }
        }
    }
}
