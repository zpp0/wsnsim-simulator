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

#include "modules.h"
#include "node.h"
#include "eventQueue.h"

class Simulator
{
public:
    Simulator();

    // ICore* getCoreInterface(IModule* receiver, QString name);
    Module* getEnvInterface(Module* receiver, QString interfaceName);
    Module* getNodeInterface(Module* receiver, Node* node, QString interfaceName);

    void registerEventHandler(Module* handler, QString eventName);

    // --
    static void postEvent(Module* author, Event* event);

    static void registerNode(Node* node);
    // --

    void init(QString projectFileName);
    void eval();

private:
    // максимально-возможное время работы симулятора
    VirtualTime m_maxGlobalTime;

    // nodes list

    static QList<Node*> m_nodes;

    static QMap<QString, QList<Module*> > m_eventHandlers;

    // Loggable events list
    // events are contained in this list can be written to log
    static QList<QString> m_loggableEvents;

    static eventQueue m_queue;
};

#endif // SIMULATOR_H
