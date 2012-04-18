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
#include "node.h"
#include "event.h"
#include "log.h"

// TODO: WHIS WILL BE DELETED!
#include "scene.h"
#include "radioChannel.h"
#include "rtx.h"
#include "timer.h"

#include "ITimer.h"

#include "projectParams.h"

VirtualTime simulator::m_maxSimulatorWorkTime;

simulator::simulator(QString projectFileName)
{
    QLibrary projectDataLib("./libprojectData");
    projectDataLib.load();

    if(!projectDataLib.isLoaded()) {
        qDebug() << "Error load library";
        return;
    }        

    // FIXME: make it easy
    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad pd = (projectDataLoad) projectDataLib.resolve("load");

    QString errorMessage;
    QString projectFile;

    ProjectParams projectParams = pd(projectFileName, &errorMessage);

    log::init(projectParams.simulatorParams.logFile, projectParams.events);

    m_maxSimulatorWorkTime = projectParams.simulatorParams.maxTime;

    // TODO: worse. Delete this
    Scene* scene = new Scene();

    radioChannel* radio = new radioChannel();

    Env::m_interfaces_TEMP["IScene"] = qobject_cast<IModule*> (scene);
    Env::m_interfaces_TEMP["IRadioChannel"] = qobject_cast<IModule*> (radio);

    QList<ModuleParam> scene_params;
    QList<ModuleParam> radioChannel_params;
    QList<ModuleParam> rtx_params;
    foreach (ModuleParams params, projectParams.modulesParams) {
        if (params.moduleName == "Scene")
            scene_params = params.params;
        if (params.moduleName == "rtx")
            rtx_params = params.params;
        if (params.moduleName == "radioChannel")
            radioChannel_params = params.params;
    }

    qDebug() << "scene init";
    scene->moduleInit(scene_params);
    // qDebug() << "interface cast radio" << qobject_cast<IModule*> (radio);

    // qDebug() << "nodes";
    QVector<Node*> nodes = scene->nodes();
    foreach(Node* node, nodes) {
        // qDebug() << "new node";
        RTX* rtx = new RTX();
        Timer* timer = new Timer();
        // qDebug() << "set parent Node to RTX" << node;
        rtx->setParentNode(node);
        timer->setParentNode(node);

        // qDebug() << "interface cast rtx" << qobject_cast<IModule*> (rtx);
        node->m_interfaces_TEMP["Irtx"] = (IModule*) rtx;
        // node->m_interfaces_TEMP["Irtx"] = qobject_cast<IModule*> (rtx);
        node->m_interfaces_TEMP["ITimer"] = (IModule*) timer;
        // node->m_interfaces_TEMP["ITimer"] = qobject_cast<IModule*> (timer);

        rtx->moduleInit(rtx_params);
    }

    qDebug() << "radio init";
    radio->moduleInit(radioChannel_params);

    // foreach (Node* node, nodes) {
    //     qDebug() << "node init" << node->ID;
    //     node->init();
    // }
    
    // QFileInfo info = QFileInfo(QDir::current(), projectFileName);
    // info.setFile(info.absolutePath());

    // if (info.exists() && info.isDir())
    //     m_currentProjectPath = info.absoluteFilePath();
    // else
    //     qFatal("ERROR: There is no directory in argv. Exit programm");

    // qDebug() << "Project path is" << m_currentProjectPath;
    
    // log::init(m_currentProjectPath + "/", format);
    // luaHandler::currentProjectPath = m_currentProjectPath;

    // loadProject(projectFileName);
}

// simulator::~simulator()
// {
// }

VirtualTime simulator::getMaxSimulatorWorkTime()
{
    return m_maxSimulatorWorkTime;
}

void simulator::loadProject (QString projectFileName)
{
    // qDebug() << "read xml file";

    // загружаем файл проекта

    // projectParams params();

    // // загружаем файл проекта
    // if (!params.loadFileProject(projectFileName))
    //     std::cerr << "Error: cannot load xml file " << projectFileName.toStdString() << std::endl
    //               << "Exit." << std::endl;

    // // загружаем информацию о проекте
    // if (!params.loadProjectInfo())
    //     std::cerr << "Error: cannot load xml file " << projectFileName.toStdString() << std::endl
    //               << "Exit." << std::endl;

    // // получаем загруженную информацию
    // projectInfo info = params.getProjectInfo();

    // // сравниваем версию симулятора с версией данных
    // if (info.projectFileVersion != m_version)
    //     std::cerr << "Warning: Version of core and xml data are differens" << std::endl;

    // // ищем параметры плагина журнала - путь к файлу журнала и его формат

    // // для всех используемых плагинов
    // foreach (pluginName in info.pluginsNames) {
    //     // загружаем параметры плагина
    //     if (params.loadPluginParams(pluginName)) {
    //         // получаем параметры плагина
    //         pluginParams pluginParams = params.getPluginParams();

    //         // для всех параметров плагина
    //         foreach (key in pluginParams.params.keys()) {
    //             // если название совпало с ожидаемым, запоминаем его
    //             if (key == "logFile")
    //                 m_logFile = pluginParams.params[key];
    //             if (key == "fileFormat")
    //                 m_fileFormat = pluginParams.params[key];
    //         }
    //     }
    // }
    

    // // устанавливаем параметры на страницах
    // // параметры среды
    // env::setEnv(params->env);
    // // параметры процессов
    // env::setProcesses(params->processes);
    // // паарметры типов узлов
    // env::setNodeTypes(params->nodeTypes);
    // // параметры узлов
    // env::setNodes(params->nodes);

    // qDebug() << "Loaded params:"
    //          << "\nENV"
    //          << "\nMax time" << params->env->maxSimulatorWorkTime
    //          << "\nTime units" << params->env->timeUnits
    //          << "\nsize" << params->env->size[0] << params->env->size[1] << params->env->size[2]
    //          << "\n"
    //          << params->processes.size() << "processes"
    //          << params->nodeTypes.size() << "nodeTypes"
    //          << params->nodes.size() << "nodes";
    
    // for (int i = 0; i < params->processes.size(); i++) {
    //     qDebug() << "PROCESSES"
    //              << "\nid" << params->processes[i]->processID
    //              << "\nname" << params->processes[i]->name
    //              << "\nunits" << params->processes[i]->units
    //              << "\nlua" << params->processes[i]->luaScript;

    //     delete params->processes[i];
    // }

    // for (int i = 0; i < params->nodeTypes.size(); i++) {
    //     qDebug() << "\nNODETYPES"
    //              << "\nid" << params->nodeTypes[i]->nodeTypeID
    //              << "\nname" << params->nodeTypes[i]->name
    //              << "\ntransmitterPower" << params->nodeTypes[i]->transmitterPower
    //              << "\nreceiverSensivity" << params->nodeTypes[i]->receiverSensivity
    //              << "\nfrequency" << params->nodeTypes[i]->frequency
    //              << "\ndirectionalAntenna" << params->nodeTypes[i]->directionalAntenna
    //              << "\nunrenewableEnergy" << params->nodeTypes[i]->unrenewableEnergy
    //              << "\nenergy" << params->nodeTypes[i]->energy
    //              << "\nmovement" << params->nodeTypes[i]->movement
    //              << "\nrealClock" << params->nodeTypes[i]->realClock;
        
    //     for (int j = 0; j < params->nodeTypes[i]->sensors.size(); j++) {

    //         qDebug() << "\nSENSORS"
    //                  << "\nsensorID" << params->nodeTypes[i]->sensors[j]->sensorID
    //                  << "\nprocessID" << params->nodeTypes[i]->sensors[j]->processID
    //                  << "\nname" << params->nodeTypes[i]->sensors[j]->name
    //                  << "\nunits" << params->nodeTypes[i]->sensors[j]->units
    //                  << "\nminValue" << params->nodeTypes[i]->sensors[j]->minValue
    //                  << "\nmaxValue" << params->nodeTypes[i]->sensors[j]->maxValue;
            
    //         delete params->nodeTypes[i]->sensors[j];
    //     }

    //     for (int j = 0; j < params->nodeTypes[i]->clockProcessDependence.size(); j++) {

    //         qDebug() << "\nPROCESSDEPENDS"
    //                  << "\nID" << params->nodeTypes[i]->clockProcessDependence[j];
            
    //     }
        
    //     delete params->nodeTypes[i];
    // }
    
    // for (int i = 0; i < params->nodes.size(); i++) {

    //     qDebug() << "\nNODES"
    //              << "\nnodeID" << params->nodes[i]->nodeID
    //              << "\nnodeTypeID" << params->nodes[i]->nodeTypeID
    //              << "\ncoord" << params->nodes[i]->coord[0] << params->nodes[i]->coord[1] << params->nodes[i]->coord[2]
    //              << "\nrandom" << params->nodes[i]->random;

        
    //     delete params->nodes[i];
    // }

    // delete params->env;
    // delete params->project;
    // delete params->simulator;
    // delete params;

}

void simulator::eval()
{
    qDebug() << "SIMULATOR START";
    qDebug() << "max time" << getMaxSimulatorWorkTime();
    
    quint64 oldTime = 0;

    int completionPersent = 0;
    
    // processableEvent* nextEvent;
    event* nextEvent;

    // node* node = NULL;

    // для всех событий из очереди
    // извлекаем событие и отправляем соответствующее сообщение
    while ((nextEvent = Env::queue.pop()) != NULL) {

        // Timer::timerInterrupt* e = (Timer::timerInterrupt*) nextEvent;
        // qDebug() << "time" << env::getMainTime() << nextEvent->startTime;

        // qDebug() << "time" << Env::time;
        qDebug() << "pop new event" << nextEvent->eventName()
                 << "number" << event::count
                 << "time" << nextEvent->time;
        
        // если текущее время равно или превышает максимальное
        if (nextEvent->time >= getMaxSimulatorWorkTime()) {
        // if (e->time >= getMaxSimulatorWorkTime()) {
            if (completionPersent < 100) {
                completionPersent = 100;
                std::cout << "\r" << std::unitbuf << completionPersent << "%";
            }
            delete nextEvent;


            int count = 1;
            while (Env::queue.pop() != NULL)
                count++;

            event::count -= count;

            qDebug() << "rest" << count;
            break;
        }

        oldTime = Env::time;

        // if ((nextEvent->startTime >= oldTime)
        //     || (node != ((nodeEvent*)nextEvent)->eventNode))
        // {
        // if (nextEvent->check() == true) {

         //устанавливаем время, на время начала события
        Env::time = nextEvent->time;
        // Env::time = e->time;

        // if (oldTime != Env::time) {
        //     if (env::moveNodes())
        //         env::nodesHearTest();
        // }

        // FIXME: переделать
        if (nextEvent->recordable)
            log::writeLog(nextEvent);

        // выполнения события
        nextEvent->process();

        // }

        // node = ((nodeEvent*)nextEvent)->eventNode;

        VirtualTime remainingTime = getMaxSimulatorWorkTime() - nextEvent->time;
        // qint64 remainingTime = getMaxSimulatorWorkTime() - e->time;
        int currentPercent = ((getMaxSimulatorWorkTime() - remainingTime) * 100) / getMaxSimulatorWorkTime();
        if (completionPersent != currentPercent) {
            completionPersent = currentPercent;
            std::cout << "\r" << std::unitbuf << completionPersent << "%";
        }
        
        delete nextEvent;
    }

    qDebug() << "events count" << event::count;
    qDebug() << "nodePowerUp" << IScene::nodePowerUp::count;
    qDebug() << "ChangeLink" << IRadioChannel::ChangeLink::count;
    qDebug() << "CCATest" << Irtx::CCATest::count;
    qDebug() << "Collision" << Irtx::Collision::count;
    qDebug() << "SFD_TX_Down" << Irtx::SFD_TX_Down::count;
    qDebug() << "SFD_TX_Up" << Irtx::SFD_TX_Up::count;
    qDebug() << "SFD_RX_Down" << Irtx::SFD_RX_Down::count;
    qDebug() << "SFD_RX_Up" << Irtx::SFD_RX_Up::count;
    qDebug() << "timer" << ITimer::timerInterrupt::count;
    

    log::uninit();
    // std::cerr << std::endl << event::Count << " events" << std::endl;
    // std::cerr << std::endl << nodeEventRadioRX::CollCount << " collisions" << std::endl;
    // std::cerr << std::endl << nodeEventRadioRX::RXCount << " RX messages" << std::endl;
    // // qDebug() << event::Count << "events";
}
