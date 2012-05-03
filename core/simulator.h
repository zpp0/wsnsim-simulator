/**
 *
 * File: simulator.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "ISimulator.h"

#include "types.h"

#include "node.h"
#include "event.h"
#include "eventQueue.h"

#include "projectParams.h"

class Simulator : public ISimulator
{
public:
    Simulator(QString projectFileName);

    /* virtual */ ICore* getCoreInterface(IModule* receiver, QString name);
    /* virtual */ QList<QString> getEvents();
    /* virtual */ void registerEventHandler(IModule* handler, QString eventName);

    // --
    static void postEvent(IModule* author, Event* event);
    
    static IModule* getEnvInterface(IModule* receiver, QString interfaceName);
    static IModule* getNodeInterface(IModule* receiver, INode* node, QString interfaceName);

    static void registerNode(Node* node);
    // --
    
    void eval();

private:
    ProjectParams loadProject(QString file);
    
    // максимально-возможное время работы симулятора
    VirtualTime m_maxGlobalTime;

    static QMap<QString, IModule*> m_envInterfaces;
    static QMap<INode*, QMap<QString, IModule*> > m_nodeInterfaces;

    // node modules
    static QMap<IModule*, Node*> m_nodesModules;

    // nodes list
    static QList<Node*> m_nodes;
    
    // Events names list
    QList<QString> m_events;

    // FIXME: is it works with IModule?
    QMap<QString, QList<IModule*> > m_eventHandlers;
    
    // Priority events list
    static QList<QString> m_priorityEvents;
    
    // Loggable events list
    // events are contained in this list can be written to log
    QList<QString> m_loggableEvents;

    // events counts
    // <eventName, count>
    static QMap<QString, quint64> m_eventCount;
    
    static eventQueue m_queue;
};

#endif // SIMULATOR_H
