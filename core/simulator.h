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
#include "node.h"
#include "eventQueue.h"
#include "project.h"

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

    static void setMaxTime(VirtualTime maxTime);
    // static NodeID nodesNumber();

    void init(QString projectFileName);
    void eval();

private:
    // максимально-возможное время работы симулятора
    static VirtualTime m_maxGlobalTime;

    static Project* m_project;

    static QMap<QString, QList<Module*> > m_eventHandlers;

    // Loggable events list
    // events are contained in this list can be written to log
    static QList<QString> m_loggableEvents;

    static eventQueue m_queue;
};

#endif // SIMULATOR_H
