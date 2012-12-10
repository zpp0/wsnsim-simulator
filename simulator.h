/**
 *
 * File: simulator.h
 * Description: Simulator evaluator
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "virtualTime.h"

#include "module.h"
#include "eventQueue.h"
#include "project.h"
#include "IHandler.h"

class Simulator
{
public:
    static void registerEventHandler(EventID eventID, IHandler* handler);
    static void registerEvent(QString name, ModuleID author, EventID event, bool recordable);
    static void registerEventParam(QString eventName,
                                   ModuleID author,
                                   QString argName,
                                   quint16 paramID,
                                   EventParamType type);

    static QMap<ModuleID, EventID> getEventID(QString name);
    static QVector<EventParam> getEventParams(QString name, ModuleID author);

    static VirtualTime globalTime();
    static void postEvent(ModuleID author,
                          QString name,
                          VirtualTime delay,
                          QVector<EventParam>& params);

    static void setMaxTime(VirtualTime maxTime);

    void init(QString projectFileName);
    void eval();

private:
    // максимально-возможное время работы симулятора
    static VirtualTime m_maxGlobalTime;

    // реальное время в системе
    static VirtualTime m_globalTime;

    static QMap<EventID, QList<IHandler*> > m_eventHandlers;

    static QMap<QString, QMap<ModuleID, EventID> > m_events;
    static QMap<QString, QMap<ModuleID, QVector<EventParam> > > m_eventsArgs;

    // Loggable events list
    // events are contained in this list can be written to log
    static QList<EventID> m_loggableEvents;

    static eventQueue m_queue;
};

#endif // SIMULATOR_H
