/**
 *
 * File: simulator.cpp
 * Description: Simulator evaluator
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QFileInfo>
#include <iostream>
#include <cassert>

#include "simulator.h"
#include "event.h"
#include "log.h"

VirtualTime Simulator::m_globalTime;
VirtualTime Simulator::m_maxGlobalTime;

QMap<EventID, QList<EventHandler*> > Simulator::m_eventHandlers;
QMap<QString, QMap<ModuleID, EventID> > Simulator::m_events;
QMap<QString, QMap<ModuleID, QVector<EventParam> > > Simulator::m_eventsArgs;

QList<EventID> Simulator::m_loggableEvents;

eventQueue Simulator::m_queue;

void Simulator::registerEvent(QString name, ModuleID author, EventID event)
{
    m_events[name][author] = event;
}

void Simulator::registerEventHandler(EventID eventID, EventHandler* handler)
{
    m_eventHandlers[eventID] += handler;
}

void Simulator::registerEventParam(QString eventName,
                                   ModuleID author,
                                   QString argName,
                                   quint16 paramID,
                                   EventParamType type)
{
    QVector<EventParam> params = m_eventsArgs[eventName][author];
    if (params.size() < paramID)
        params.reserve(paramID);

    EventParam param;
    param.ID = paramID;
    param.name = argName;
    param.type = type;

    m_eventsArgs[eventName][author].insert(paramID, param);
}

QMap<ModuleID, EventID> Simulator::getEventID(QString name)
{
    return m_events[name];
}

QVector<EventParam> Simulator::getEventParams(QString name, ModuleID author)
{
    return m_eventsArgs[name][author];
}

VirtualTime Simulator::globalTime()
{
    return m_globalTime;
}

void Simulator::postEvent(ModuleID author, QString name, VirtualTime delay, QVector<EventParam> params)
{
    // TODO: bufferization
    Event* event = new Event();
    event->name = name;
    event->author = author;
    event->time = m_globalTime + delay;
    if (delay == 0)
        event->recordable = false;
    else
        event->recordable = true;
    event->params = params;

    postEvent(event);
};

void Simulator::postEvent(Event* event)
{
    event->ID = m_events[event->name][event->author];

    if (event->recordable == false)
        // if (m_loggableEvents.contains(event->ID))
            Log::write(event);

    // TODO: add author argument?
    // TODO: add priority argument
    m_queue.insert(event);
}

void Simulator::setMaxTime(VirtualTime maxTime)
{
    m_maxGlobalTime = maxTime;
}

void Simulator::init(QString projectFileName)
{
    Project project(projectFileName);
    // FIXME: is it always works?
    if (!project.load()
        || !project.initSimulator()
        || !project.initLog()
        || !project.initLua()
        || !project.loadModules()
        || !project.createModules()
        || !project.initModules())
    {
        std::cout << qPrintable(project.errorString());
        exit(1);
    }
}

void Simulator::eval()
{
    qDebug() << "SIMULATOR START";
    qDebug() << "max time" << m_maxGlobalTime;

    quint64 oldTime = 0;

    int completionPersent = 0;

    Event* nextEvent;

    // для всех событий из очереди
    // извлекаем событие и отправляем соответствующее сообщение
    while ((nextEvent = m_queue.pop()) != NULL) {

        // qDebug() << "time" << Env::time;
        qDebug() << "pop new event" << nextEvent->name
                 << "id" << nextEvent->ID
                 << "time" << nextEvent->time
                 << "params" << nextEvent->params.size();

        // если текущее время равно или превышает максимальное
        if (nextEvent->time >= m_maxGlobalTime) {
            if (completionPersent < 100) {
                completionPersent = 100;
                std::cout << "\r" << completionPersent << "%" << std::unitbuf;
            }
            delete nextEvent;

            break;
        }

        oldTime = m_globalTime;

         //устанавливаем время, на время начала события
        m_globalTime = nextEvent->time;

        if (m_globalTime > oldTime) {
            // TODO: generate priority not loggable event timeChanged()
        }

        if (nextEvent->recordable == true)
            // if (m_loggableEvents.contains(nextEvent->ID))
                Log::write(nextEvent);

        foreach(EventHandler* handler, m_eventHandlers[nextEvent->ID])
            handler->handle(nextEvent);

        VirtualTime remainingTime = m_maxGlobalTime - nextEvent->time;
        int currentPercent = ((m_maxGlobalTime - remainingTime) * 100) / m_maxGlobalTime;

        if (completionPersent != currentPercent) {
            completionPersent = currentPercent;
            std::cout << "\r" << completionPersent << "%" << std::unitbuf;
        }

        delete nextEvent;
    }

    // TODO: to separate initialisation and uninitialisation from evaluation
    Log::uninit();
}
