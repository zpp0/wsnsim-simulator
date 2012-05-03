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

#include "simulator.h"
#include "env.h"
#include "event.h"
#include "log.h"

// TODO: simplify it
QMap<QString, IModule*> Simulator::m_envInterfaces;
QMap<INode*, QMap<QString, IModule*> > Simulator::m_nodeInterfaces;
QList<QString> Simulator::m_priorityEvents;
QMap<IModule*, Node*> Simulator::m_nodesModules;
QList<Node*> Simulator::m_nodes;

QMap<QString, quint64> Simulator::m_eventCount;

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

    return interface;
}

IModule* Simulator::getNodeInterface(IModule* receiver, INode* node, QString interfaceName)
{
    IModule* interface = NULL;

    if (receiver->moduleInfo.importInterfaces.contains(interfaceName))
        interface = m_nodeInterfaces[node][interfaceName];

    return interface;
}

void Simulator::registerNode(Node* node)
{
    m_nodes += node;
}

Simulator::Simulator(QString projectFileName)
{

    ProjectParams projectParams = loadProject(projectFileName);

    // getting events <name, ID>
    QMap<QString, EventID> events;

    // TODO: now we can count events
    
    foreach (EventParams params, projectParams.events.systemEvents) {
        QString eventName = params.eventInfo["name"];
        EventID eventID = params.eventInfo["ID"].toUInt();
        events[eventName] = eventID;
    }

    Log::init(projectParams.simulatorParams.logFile, events);

    m_maxGlobalTime = projectParams.simulatorParams.maxTime;

    // // TODO: worse. Delete this
    // Scene* scene = new Scene();

    // radioChannel* radio = new radioChannel();

    // Env::m_interfaces_TEMP["IScene"] = qobject_cast<IModule*> (scene);
    // Env::m_interfaces_TEMP["IRadioChannel"] = qobject_cast<IModule*> (radio);

    // QList<ModuleParam> scene_params;
    // QList<ModuleParam> radioChannel_params;
    // QList<ModuleParam> rtx_params;
    // foreach (ModuleParams params, projectParams.modulesParams) {
    //     if (params.moduleName == "Scene")
    //         scene_params = params.params;
    //     if (params.moduleName == "rtx")
    //         rtx_params = params.params;
    //     if (params.moduleName == "radioChannel")
    //         radioChannel_params = params.params;
    // }

    // qDebug() << "scene init";
    // scene->moduleInit(scene_params);
    // // qDebug() << "interface cast radio" << qobject_cast<IModule*> (radio);

    // // qDebug() << "nodes";
    // QVector<Node*> nodes = scene->nodes();
    // foreach(Node* node, nodes) {
    //     // qDebug() << "new node";
    //     RTX* rtx = new RTX();
    //     Timer* timer = new Timer();
    //     // qDebug() << "set parent Node to RTX" << node;
    //     rtx->setParentNode(node);
    //     timer->setParentNode(node);

    //     // qDebug() << "interface cast rtx" << qobject_cast<IModule*> (rtx);
    //     node->m_interfaces_TEMP["Irtx"] = (IModule*) rtx;
    //     // node->m_interfaces_TEMP["Irtx"] = qobject_cast<IModule*> (rtx);
    //     node->m_interfaces_TEMP["ITimer"] = (IModule*) timer;
    //     // node->m_interfaces_TEMP["ITimer"] = qobject_cast<IModule*> (timer);

    //     rtx->moduleInit(rtx_params);
    // }

    // qDebug() << "radio init";
    // radio->moduleInit(radioChannel_params);
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

        // // TODO: do it
        // foreach (IModule* handler, m_eventHandlers[nextEvent->name])
        //     handler->eventProcess(nextEvent);

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
