/**
 *
 * File: env.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "env.h"

VirtualTime Env::time;
eventQueue Env::queue;
QMap<QString, IModule*> Env::m_interfaces_TEMP;

IModule* Env::getInterface(IModule* module, QString interface)
{
    // qDebug() << "test0";    
    // FIXME: duplicate code in node.cpp
    // IModule* Interface = NULL;
    // QList<QString> interfaces = module->moduleImportInterfaces();
    // foreach (QString intname, interfaces)
        // qDebug() << intname;

    // if (module->moduleImportInterfaces().indexOf(interface) != -1)
    // Interface = m_interfaces_TEMP[interface];
    return m_interfaces_TEMP[interface];

    // qDebug() << Interface;
    // return Interface;
}
