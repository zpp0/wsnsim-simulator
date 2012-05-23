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
    /* virtual */ IModule* getEnvInterface(IModule* receiver, QString interfaceName);
    /* virtual */ IModule* getNodeInterface(IModule* receiver, INode* node, QString interfaceName);

    /* virtual */ QList<QString> getEvents();
    /* virtual */ void registerEventHandler(IModule* handler, QString eventName);

    // --
    static void postEvent(IModule* author, Event* event);
    
    static void registerNode(Node* node);
    // --
    
    void eval();

private:
    static ISimulator* m_this;
    
    ProjectParams loadProject(QString file);
    
    // максимально-возможное время работы симулятора
    VirtualTime m_maxGlobalTime;

    static QMap<QString, IModule*> m_envInterfaces;
    static QMap<INode*, QMap<QString, IModule*> > m_nodeInterfaces;

    // node modules
    static QList<QString> m_nodesLoaders;
    static QMap<IModule*, Node*> m_nodesModules;

    // nodes list
    static QList<Node*> m_nodes;
    
    // Events names list
    QList<QString> m_events;

    static QMap<QString, QList<IModule*> > m_eventHandlers;
    
    // Priority events list
    static QList<QString> m_priorityEvents;
    
    // Loggable events list
    // events are contained in this list can be written to log
    QList<QString> m_loggableEvents;

    // events counts
    // <eventName, count>
    static QMap<QString, quint64> m_eventCount;
    
    static eventQueue m_queue;

    static ProjectParams m_projectParams;
};

#endif // SIMULATOR_H
