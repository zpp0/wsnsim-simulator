/**
 *
 * File: simulator.h
 * Description: Simulator evaluator
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "types.h"

#include "module.h"
#include "eventQueue.h"
#include "project.h"
#include "eventHandler.hpp"

class Simulator
{
public:
    static void registerEventHandler(EventID eventID, EventHandler handler);
    static void registerEvent(QString name, ModuleID author, EventID event);

    static QMap<ModuleID, EventID> getEventID(QString name);

    static VirtualTime globalTime();
    static void postEvent(Event* event);

    static void setMaxTime(VirtualTime maxTime);

    void init(QString projectFileName);
    void eval();

private:
    // максимально-возможное время работы симулятора
    static VirtualTime m_maxGlobalTime;

    // реальное время в системе
    static VirtualTime m_globalTime;

    static Project* m_project;

    static QMap<EventID, QList<EventHandler> > m_eventHandlers;

    static QMap<QString, QMap<ModuleID, EventID> > m_events;

    // Loggable events list
    // events are contained in this list can be written to log
    static QList<EventID> m_loggableEvents;

    static eventQueue m_queue;
};

#endif // SIMULATOR_H
