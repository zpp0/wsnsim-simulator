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

class Simulator
{
public:
    // void registerEventHandler(ModuleID handler, QString eventName);

    static VirtualTime globalTime();
    static void postEvent(ModuleID author, Event* event);

    static void setMaxTime(VirtualTime maxTime);

    void init(QString projectFileName);
    void eval();

private:
    // максимально-возможное время работы симулятора
    static VirtualTime m_maxGlobalTime;

    // реальное время в системе
    static VirtualTime m_globalTime;

    static Project* m_project;

    static QMap<QString, QList<Module*> > m_eventHandlers;

    // Loggable events list
    // events are contained in this list can be written to log
    static QList<QString> m_loggableEvents;

    static eventQueue m_queue;
};

#endif // SIMULATOR_H
