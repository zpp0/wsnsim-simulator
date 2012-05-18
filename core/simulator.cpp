/**
 *
 * File: simulator.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <iostream>
#include <cassert>

#include <QPluginLoader>

#include "simulator.h"
#include "env.h"
#include "event.h"
#include "log.h"

// TODO: simplify it
QMap<QString, IModule*> Simulator::m_envInterfaces;
QMap<INode*, QMap<QString, IModule*> > Simulator::m_nodeInterfaces;
QList<QString> Simulator::m_priorityEvents;
QList<QPluginLoader*> Simulator::m_nodesLoaders;
QMap<IModule*, Node*> Simulator::m_nodesModules;
QList<Node*> Simulator::m_nodes;
QMap<QString, QList<IModule*> > Simulator::m_eventHandlers;
// FIXME: delete this
ISimulator* Simulator::m_this;
QMap<QString, quint64> Simulator::m_eventCount;
ProjectParams Simulator::m_projectParams;

eventQueue Simulator::m_queue;

ICore* Simulator::getCoreInterface(IModule* receiver, QString name)
{
    ICore* interface = NULL;

    // TODO: do interfaces as singletons

    if (receiver->moduleInfo.importInterfaces.contains(name)) {

        // TODO: throw exception if module has no permissions to get interface

        if (name == "IEnv")
            interface = (ICore*)new Env();
        if (name == "IEvent")
            interface = (ICore*)new EventFactory();
        if (name == "INodesFactory")
            interface = (ICore*)new NodesFactory();
        if (name == "INode")
            interface = (ICore*)(INode*)m_nodesModules[receiver];
    }

    qDebug() << "module" << receiver->moduleInfo.name << "get interface" << name << interface;
    
    return interface;
}

QList<QString> Simulator::getEvents()
{
    return m_events;
}

void Simulator::registerEventHandler(IModule* handler, QString eventName)
{
    m_eventHandlers[eventName] += handler;
}

void Simulator::postEvent(IModule* author, Event* event)
{
    if (m_priorityEvents.contains(event->name)) {
        // TODO: notify queue about it
    }


    // TODO: add author argument?
    // TODO: add priority argument
    m_queue.insert(event);
    
    // TODO: count loggable events
    m_eventCount[event->name] += 1;
}

IModule* Simulator::getEnvInterface(IModule* receiver, QString interfaceName)
{
    IModule* interface = NULL;

    if (receiver->moduleInfo.importInterfaces.contains(interfaceName))
        interface = m_envInterfaces[interfaceName];

    qDebug() << "module" << receiver->moduleInfo.name << "get interface" << interfaceName << interface;
    
    return interface;
}

IModule* Simulator::getNodeInterface(IModule* receiver, INode* node, QString interfaceName)
{
    IModule* interface = NULL;

    if (receiver->moduleInfo.importInterfaces.contains(interfaceName))
        interface = m_nodeInterfaces[node][interfaceName];

    qDebug() << "module" << receiver->moduleInfo.name << "get interface" << interfaceName << interface;
    
    return interface;
}

void Simulator::registerNode(Node* node)
{
    m_nodes += node;
    foreach (QPluginLoader* loader, m_nodesLoaders) {
        QObject* plugin = loader->instance();
        IModule* module = qobject_cast<IModule *>(plugin);

        m_nodesModules[module] = node;

        m_nodeInterfaces[node][module->interfaceInfo.name] = module;

    }

    foreach (IModule* module, m_nodesModules.keys()) {
        QMap<QString, QString> params;
        foreach (ModuleParams moduleParams, m_projectParams.modulesParams) {
            if (moduleParams.moduleName == module->moduleInfo.name) {
                foreach(ModuleParam param, moduleParams.params)
                    params[param.name] = param.value;
            }
        }

        qDebug() << "init" << module->moduleInfo.name;

        module->moduleInit(m_this, params);

        foreach(QString event, module->moduleInfo.handledEvents)
            m_eventHandlers[event] += module;
    }
}

Simulator::Simulator(QString projectFileName)
{
    m_this = this;
    
    m_projectParams = loadProject(projectFileName);

    // getting events <name, ID>
    QMap<QString, EventID> events;

    // TODO: now we can count events
    
    foreach (EventParams params, m_projectParams.events.systemEvents) {
        QString eventName = params.eventInfo["name"];
        EventID eventID = params.eventInfo["ID"].toUInt();
        events[eventName] = eventID;
        m_events += eventName;
    }

    Log::init(m_projectParams.simulatorParams.logFile, events);

    m_loggableEvents = m_events;
        
    m_maxGlobalTime = m_projectParams.simulatorParams.maxTime;

    QDir myDir(QDir::currentPath() + "/modules");
    QStringList plugins = myDir.entryList(QStringList() << "*.so" << "*.dll");

    qDebug() << "plugins" << plugins;
    
    foreach(QString plugin, plugins) {
        QPluginLoader* loader = new QPluginLoader(QDir::currentPath() + "/modules/" + plugin);
        QObject* plugin = loader->instance();
        IModule* module = qobject_cast<IModule *>(plugin);
        
        qDebug() << module << module->interfaceInfo.type;
        
        if (module->interfaceInfo.type == InterfaceType_Environment) {
            m_envInterfaces[module->interfaceInfo.name] = module;

            qDebug() << "env module init" << module->moduleInfo.name;

        }
        else if (module->interfaceInfo.type == InterfaceType_Hardware
                 || module->interfaceInfo.type == InterfaceType_Software) {
            qDebug() << "node module init wirh delay" << module->moduleInfo.name;            
            m_nodesLoaders += loader;
            delete module;
        }
    }
    
    foreach(IModule* module, m_envInterfaces) {
        QMap<QString, QString> params;
        foreach (ModuleParams moduleParams, m_projectParams.modulesParams) {
            if (moduleParams.moduleName == module->moduleInfo.name) {
                foreach(ModuleParam param, moduleParams.params)
                    params[param.name] = param.value;
            }
        }

        qDebug() << "init" << module->moduleInfo.name;

        module->moduleInit(this, params);

        foreach(QString event, module->moduleInfo.handledEvents)
            m_eventHandlers[event] += module;
    }
}

ProjectParams Simulator::loadProject (QString projectFileName)
{
    QLibrary projectDataLib("./libprojectData");
    projectDataLib.load();

    if(!projectDataLib.isLoaded()) {
        // TODO: throw good exception
        qDebug() << "Error load library";
        throw;
        // return projectParams;
    }

    // FIXME: make it easy
    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad pd = (projectDataLoad) projectDataLib.resolve("load");

    QString errorMessage;
    QString projectFile;

    ProjectParams projectParams = pd(projectFileName, &errorMessage);

    // TODO: check errors

    return projectParams;
}

void Simulator::eval()
{
    qDebug() << "SIMULATOR START";
    qDebug() << "max time" << m_maxGlobalTime;
    
    quint64 oldTime = 0;

    int completionPersent = 0;
    
    Event* nextEvent;

    foreach(INode* node, m_nodeInterfaces.keys())
        qDebug() << "nodeID" << node->ID();
    
    // для всех событий из очереди
    // извлекаем событие и отправляем соответствующее сообщение
    while ((nextEvent = m_queue.pop()) != NULL) {

        // qDebug() << "time" << Env::time;
        qDebug() << "pop new event" << nextEvent->name
                 // << "number" << event::count
                 << "time" << nextEvent->time;
        
        // если текущее время равно или превышает максимальное
        if (nextEvent->time >= m_maxGlobalTime) {
            if (completionPersent < 100) {
                completionPersent = 100;
                std::cout << "\r" << std::unitbuf << completionPersent << "%";
            }
            delete nextEvent;


            // int count = 1;
            // while (Env::queue.pop() != NULL)
            //     count++;

            // event::count -= count;

            // qDebug() << "rest" << count;

            break;
        }

        oldTime = Env::time;

         //устанавливаем время, на время начала события
        Env::time = nextEvent->time;

        if (Env::time > oldTime) {
            // TODO: generate priority not loggable event timeChanged()
        }

        if (m_loggableEvents.contains(nextEvent->name))
            Log::write(nextEvent);

        foreach (IModule* handler, m_eventHandlers[nextEvent->name]) {
            qDebug() << "handler" << handler << handler->moduleInfo.name;
        }
        
        foreach (IModule* handler, m_eventHandlers[nextEvent->name]) {
            if (!m_nodesModules.contains(handler))
                handler->eventHandler(nextEvent->name, nextEvent->params);
            
            else if (m_nodesModules.contains(handler)
                     && m_nodesModules[handler]->ID() == nextEvent->params[0].toInt()) {

                qDebug() << "module" << handler->moduleInfo.name
                         << "event nodeID" << nextEvent->params[0].toInt()
                         << "nodeID" << m_nodesModules[handler]->ID()
                         << "try to handle event" << nextEvent->name
                         << "time" << Env::time;

                handler->eventHandler(nextEvent->name, nextEvent->params);
            }
        }
        
        VirtualTime remainingTime = m_maxGlobalTime - nextEvent->time;
        int currentPercent = ((m_maxGlobalTime - remainingTime) * 100) / m_maxGlobalTime;

        if (completionPersent != currentPercent) {
            completionPersent = currentPercent;
            std::cout << "\r" << std::unitbuf << completionPersent << "%";
        }
        
        delete nextEvent;
    }

    // qDebug() << "events count" << event::count;
    // qDebug() << "nodePowerUp" << IScene::nodePowerUp::count;
    // qDebug() << "ChangeLink" << IRadioChannel::ChangeLink::count;
    // qDebug() << "CCATest" << Irtx::CCATest::count;
    // qDebug() << "Collision" << Irtx::Collision::count;
    // qDebug() << "SFD_TX_Down" << Irtx::SFD_TX_Down::count;
    // qDebug() << "SFD_TX_Up" << Irtx::SFD_TX_Up::count;
    // qDebug() << "SFD_RX_Down" << Irtx::SFD_RX_Down::count;
    // qDebug() << "SFD_RX_Up" << Irtx::SFD_RX_Up::count;
    // qDebug() << "timer" << ITimer::timerInterrupt::count;

    // TODO: to separate initialisation and uninitialisation from evaluation
    Log::uninit();

}
