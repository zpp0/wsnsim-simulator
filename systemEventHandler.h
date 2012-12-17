/**
 *
 * File: systemEventHandler.h
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#ifndef SYSTEMEVENTHANDLER_H
#define SYSTEMEVENTHANDLER_H

#include <QtCore>

#include "module.h"

class SystemEventHandler
{
public:
    SystemEventHandler(ModuleID moduleID, ModuleInstanceID ID, QString name);

    void handle(QVector<EventParam>& params);

private:
    QString m_name;
    ModuleID m_moduleID;
    ModuleInstanceID m_ID;
};

#endif // SYSTEMEVENTHANDLER_H
