/**
 *
 * File: IHandler.h
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#ifndef IHANDLER_H
#define IHANDLER_H

#include <QtCore>

#include "module.h"
#include "event.h"

class IHandler
{
public:
    IHandler(ModuleID moduleID, ModuleInstanceID ID);
    virtual ~IHandler() {}

    virtual void handle(Event* event) = 0;

    void setCustomParams(QVector<EventParam>& params);

protected:
    void setCustomValues(QVector<EventParam>& params);

    ModuleID m_moduleID;
    ModuleInstanceID m_ID;
    QVector<EventParam> m_params;
    bool withCustomParams;
};

#endif // IHANDLER_H
