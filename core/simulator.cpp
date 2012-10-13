/**
 *
 * File: simulator.cpp
 * Description: Simulator evaluator
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <iostream>
#include <cassert>

#include "simulator.h"
#include "env.h"
#include "event.h"
#include "log.h"

QList<Node*> Simulator::m_nodes;
QMap<QString, QList<Module*> > Simulator::m_eventHandlers;

QList<QString> Simulator::m_loggableEvents;

eventQueue Simulator::m_queue;

// ICore* Simulator::getCoreInterface(IModule* receiver, QString name)
// {
//     ICore* interface = NULL;

//     // TODO: do interfaces as singletons

//     if (receiver->moduleInfo.importInterfaces.contains(name)) {

//         // TODO: throw exception if module has no permissions to get interface

//         if (name == "IEnv")
//             interface = (ICore*)new Env();
//         if (name == "IEvent")
//             interface = (ICore*)new EventFactory();
//         if (name == "INodesFactory")
//             interface = (ICore*)new NodesFactory();
//         if (name == "INode")
//             interface = (ICore*)(INode*)m_nodesModules[receiver];
//     }

//     // qDebug() << "module" << receiver->moduleInfo.name << "get interface" << name << interface;

//     return interface;
// }

void Simulator::registerEventHandler(Module* handler, QString eventName)
{
    m_eventHandlers[eventName] += handler;
}

void Simulator::postEvent(Module* author, Event* event)
{

    // if (m_priorityEvents.contains(event->name)) {
    //     // TODO: notify queue about it
    // }

    if (event->recordable == false)
        if (m_loggableEvents.contains(event->name))
            Log::write(event);

    // TODO: add author argument?
    // TODO: add priority argument
    m_queue.insert(event);
}

Module* Simulator::getEnvInterface(Module* receiver, QString interfaceName)
{
    Module* interface = NULL;

    // if (receiver->moduleInfo.importInterfaces.contains(interfaceName))
    //     interface = m_envInterfaces[interfaceName];

    // qDebug() << "module" << receiver->moduleInfo.name << "get interface" << interfaceName << interface;

    return interface;
}

Module* Simulator::getNodeInterface(Module* receiver, Node* node, QString interfaceName)
{
    Module* interface = NULL;

    // if (receiver->moduleInfo.importInterfaces.contains(interfaceName))
    //     interface = m_nodeInterfaces[node][interfaceName];

    // qDebug() << "module" << receiver->moduleInfo.name << "get interface" << interfaceName << interface;

    return interface;
}

void Simulator::registerNode(Node* node)
{
    m_nodes += node;

    qDebug() << "new node" << node;

    // foreach (QString plugin, m_nodesLoaders) {
    //     QString path = QDir::currentPath() + "/.trash/" + plugin + QString::number(node->ID());
    //     QFile file(QDir::currentPath() + "/modules/" + plugin);
    //     file.copy(path);
    //     QPluginLoader loader(path);
    //     QObject* plugin = loader.instance();
    //     IModule* module = qobject_cast<IModule *>(plugin);

    //     qDebug() << "created module" << module << module->moduleInfo.name << "for node" << node->ID();

    //     m_nodesModules[module] = node;

    //     m_nodeInterfaces[node][module->interfaceInfo.name] = module;

    // }

    // // foreach (IModule* module, m_nodesModules.keys()) {
    // foreach (IModule* module, m_nodeInterfaces[node].values()) {
    //     QMap<QString, QString> params;
    //     foreach (ModuleParams moduleParams, m_projectParams.modulesParams) {
    //         if (moduleParams.moduleName == module->moduleInfo.name) {
    //             foreach(ModuleParam param, moduleParams.params)
    //                 params[param.name] = param.value;
    //         }
    //     }

    //     qDebug() << "init" << module << module->moduleInfo.name;

    //     module->moduleInit(m_this, params);

    //     foreach(QString event, module->moduleInfo.handledEvents)
    //         m_eventHandlers[event] += module;
    // }
}

Simulator::Simulator()
{
    // m_this = this;

    // m_maxGlobalTime = m_projectParams.simulatorParams.maxTime;

    // switch (m_projectParams.simulatorParams.timeUnits) {
    // case years:
    //     m_maxGlobalTime *= 12;
    // case months:
    //     m_maxGlobalTime *= 30;
    // case w:
    //     m_maxGlobalTime *= 7;
    // case d:
    //     m_maxGlobalTime *= 24;
    // case h:
    //     m_maxGlobalTime *= 60;
    // case m:
    //     m_maxGlobalTime *= 60;
    // case s:
    //     m_maxGlobalTime *= 1000;
    // case ms:
    //     m_maxGlobalTime *= 1000;
    // default:
    //     break;
    // }

    // QDir modulesDir(QDir::currentPath() + "/modules");

    // foreach(Module module, m_projectParams.modules) {
    //     if (module.moduleInfo["lang"] == "cpp") {

    //         QPluginLoader loader (QDir::currentPath() + "/modules/" + module.fileName);
    //         QObject* plugin = loader.instance();

    //         if (plugin == NULL) {
    //             std::cout << "Can't load module " << module.name << loader.errorString();
    //             // TODO: return error
    //             return 1;
    //         }

    //         IModule* module = qobject_cast<IModule *>(plugin);

    //         if (module == NULL) {
    //             std::cout << "There is no module " << module.name << " in file " << module.fileName;
    //             // TODO: return error
    //             return 1;
    //         }

    // //     if (module->interfaceInfo.type == InterfaceType_Environment) {
    // //         m_envInterfaces[module->interfaceInfo.name] = module;

    // //         qDebug() << "env module init" << module->moduleInfo.name;

    // //     }
    // //     else if (module->interfaceInfo.type == InterfaceType_Hardware
    // //              || module->interfaceInfo.type == InterfaceType_Software) {
    // //         qDebug() << "node module init wirh delay" << module->moduleInfo.name;
    // //         m_nodesLoaders += pluginName;
    // //         delete module;
    // //     }
    // // }

    //     }

    //     else if (module.moduleInfo["lang"] == "lua") {
    //     }
    // }

    // // foreach(QString pluginName, plugins) {
    // //     QPluginLoader* loader = new QPluginLoader(QDir::currentPath() + "/modules/" + pluginName);
    // //     QObject* plugin = loader->instance();
    // //     qDebug() << plugin << loader->errorString();

    // //     IModule* module = qobject_cast<IModule *>(plugin);

    // //     qDebug() << module << module->interfaceInfo.type;

    // //     if (module->interfaceInfo.type == InterfaceType_Environment) {
    // //         m_envInterfaces[module->interfaceInfo.name] = module;

    // //         qDebug() << "env module init" << module->moduleInfo.name;

    // //     }
    // //     else if (module->interfaceInfo.type == InterfaceType_Hardware
    // //              || module->interfaceInfo.type == InterfaceType_Software) {
    // //         qDebug() << "node module init wirh delay" << module->moduleInfo.name;
    // //         m_nodesLoaders += pluginName;
    // //         delete module;
    // //     }
    // // }

    // foreach(IModule* module, m_envInterfaces) {
    //     QMap<QString, QString> params;
    //     foreach (ModuleParams moduleParams, m_projectParams.modulesParams) {
    //         if (moduleParams.moduleName == module->moduleInfo.name) {
    //             foreach(ModuleParam param, moduleParams.params)
    //                 params[param.name] = param.value;
    //         }
    //     }

    //     qDebug() << "init" << module->moduleInfo.name;

    //     module->moduleInit(this, params);

    //     foreach(QString event, module->moduleInfo.handledEvents)
    //         m_eventHandlers[event] += module;
    // }
}

void Simulator::init(QString projectFileName)
{
}

void Simulator::eval()
{
    qDebug() << "SIMULATOR START";
    qDebug() << "max time" << m_maxGlobalTime;

    quint64 oldTime = 0;

    int completionPersent = 0;

    Event* nextEvent;

    // foreach(INode* node, m_nodeInterfaces.keys())
    //     qDebug() << "nodeID" << node->ID();

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
                std::cout << "\r" << completionPersent << "%" << std::unitbuf;
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

        if (nextEvent->recordable == true)
            if (m_loggableEvents.contains(nextEvent->name))
                Log::write(nextEvent);

        // foreach (Module* handler, m_eventHandlers[nextEvent->name]) {
        //     if (!m_nodesModules.contains(handler))
        //         handler->eventHandler(nextEvent->name, nextEvent->params);

        //     else if (m_nodesModules.contains(handler)
        //              && m_nodesModules[handler]->ID() == nextEvent->params[0].toInt()) {

        //         // qDebug() << "module" << handler->moduleInfo.name
        //         //          << "event nodeID" << nextEvent->params[0].toInt()
        //         //          << "nodeID" << m_nodesModules[handler]->ID()
        //         //          << "try to handle event" << nextEvent->name
        //         //          << "time" << Env::time;

        //         handler->eventHandler(nextEvent->name, nextEvent->params);
        //     }
        // }

        VirtualTime remainingTime = m_maxGlobalTime - nextEvent->time;
        int currentPercent = ((m_maxGlobalTime - remainingTime) * 100) / m_maxGlobalTime;

        if (completionPersent != currentPercent) {
            completionPersent = currentPercent;
            std::cout << "\r" << completionPersent << "%" << std::unitbuf;
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

    // QString trashDirPath = QDir::currentPath() + "/.trash";
    // QDir trash(trashDirPath);

    // foreach (QString fileName, trash.entryList(QDir::NoDotAndDotDot | QDir::Files))
    //     if (!trash.remove(fileName))
    //         qDebug() << "can't delete file" << fileName;

    // if (!trash.rmdir(trashDirPath))
    //     qDebug() << "can't delete trash directory";

}
