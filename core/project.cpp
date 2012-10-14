/**
 *
 * File: project.cpp
 * Description: initialization of simulator's project
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QDir>
#include <QFile>

#include "project.h"
#include "simulator.h"
#include "log.h"
#include "luaHost.h"
#include "moduleAdapter_lua.h"

Project::Project(QString projectFileName)
{
    m_projectFileName = projectFileName;
}

QString Project::errorString()
{
    return m_errorString;
}

int Project::load()
{
    QLibrary projectDataLibrary("./projectData");

    if (!projectDataLibrary.load()) {
        m_errorString = projectDataLibrary.errorString();
        return 0;
    }

    // FIXME: make it easy
    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad pd = (projectDataLoad) projectDataLibrary.resolve("load");

    m_projectParams = pd(m_projectFileName, &m_errorString);

    // if error, than return
    if (m_errorString != "")
        return 0;

    return 1;

    // TODO: check errors
}

int Project::initSimulator()
{
    VirtualTime maxTime = m_projectParams.simulatorParams.maxTime;

    switch (m_projectParams.simulatorParams.timeUnits) {
    case years:
        maxTime *= 12;
    case months:
        maxTime *= 30;
    case w:
        maxTime *= 7;
    case d:
        maxTime *= 24;
    case h:
        maxTime *= 60;
    case m:
        maxTime *= 60;
    case s:
        maxTime *= 1000;
    case ms:
        maxTime *= 1000;
    default:
        break;
    }

    Simulator::setMaxTime(maxTime);
    return 1;
}

bool argsSort(const QMap<QString,QString>  &s1, const QMap<QString,QString> &s2)
{
    return s1["ID"].toInt() < s2["ID"].toInt();
}

int Project::initLog()
{
    // getting events <name, ID>
    QMap<QString, EventID> events;
    QMap<QString, QList<QString > > eventArgTypes;

    // FIXME: wtf is this?
    foreach (EventParams params, m_projectParams.events.systemEvents) {
        QString eventName = params.eventInfo["name"];
        EventID eventID = params.eventInfo["ID"].toUInt();
        events[eventName] = eventID;
        QList<EventArgument> arguments = params.arguments;
        qSort(arguments.begin(), arguments.end(), argsSort);
        foreach (EventArgument arg, arguments)
            eventArgTypes[eventName] += arg["type"];
    }

    Log::init(m_projectParams.simulatorParams.logFile, events, eventArgTypes);

    return 1;
}

#ifdef MODULES_ENABLED
#ifdef LUA_ENABLED
int Project::initLua()
{
    LuaHost::open();
    return 1;
}
#endif
#endif

#ifdef MODULES_ENABLED
int Project::loadModules()
{
    QDir modulesDir(QDir::currentPath() + "/modules");

    foreach(ModuleData module, m_projectParams.modules) {

        ModuleAdapter* loader;

        // prepare module type
        ModuleType type;
        QString moduleType;
        if (moduleType == "environment")
            type = ModuleType_Environment;
        else if (moduleType == "hardware")
            type = ModuleType_Hardware;
        else if (moduleType == "software")
            type = ModuleType_Software;
        else {
            m_errorString = "Wrong type" + moduleType + "of module" + module.moduleInfo["name"];
            return 0;
        }

        // prepare module params
        QVariantMap params;
        foreach(ModuleParam param, module.params)
            params[param.name] = param.value;

        // prepare moduleID
        ModuleID ID = module.moduleInfo["ID"].toInt();

        ModuleInitData moduleInitData;
        moduleInitData.name = module.moduleInfo["name"];
        moduleInitData.ID = ID;
        moduleInitData.type = type;
        moduleInitData.fileName = module.fileName;
        moduleInitData.params = params;
        moduleInitData.dependencies = module.dependencies;

        if (module.moduleInfo["lang"] == "cpp") {
            // TODO: implement this
        }

#ifdef LUA_ENABLED
        else if (module.moduleInfo["lang"] == "lua")
            loader = (ModuleAdapter*) new ModuleAdapterLua(moduleInitData);
#endif

        if (loader == NULL) {
            m_errorString = "Can't load module" + module.moduleInfo["name"];
            return 0;
        }

        if (loader->load()) {
            if (type == ModuleType_Environment)
                m_envAdapters += loader;
            else
                m_nodeAdapters += loader;

            m_moduleType[ID] = ModuleType_Environment;
        }
        else {
            m_errorString = loader->errorString();
            return 0;
        }
    }

    return 1;
}

int Project::initModules()
{
    // creating env modules
    foreach(ModuleAdapter* envModule, m_envAdapters) {

        Module* newModule = envModule->create();
        if (newModule)
            m_envModules[envModule->ID()] = newModule;
        else {
            m_errorString = envModule->errorString();
            return 0;
        }
    }

    QList<ModuleAdapter*> uninitEnvModules;

    // init env modules without depending on the nodes
    foreach(ModuleAdapter* envModule, m_envAdapters) {

        QList<ModuleID> dependencies = envModule->dependencies();
        QList<Module*> depModules;

        foreach(ModuleID moduleID, dependencies) {
            if (m_moduleType[moduleID] != ModuleType_Environment) {
                uninitEnvModules += envModule;
                break;
            }

            depModules += m_envModules[moduleID];
        }

        envModule->init(m_envModules[envModule->ID()], depModules);
    }

    // now we must have more than 0 registered nodes
    quint16 nodesNum = Simulator::nodesNumber();

    if (nodesNum == 0) {
        m_errorString = "There are no registered nodes in simulator. Can't continue.";
        return 0;
    }

    // creating nodes modules
    for (quint16 ID = 0; ID < nodesNum; ID++) {
        foreach(ModuleAdapter* nodeModule, m_nodeAdapters) {

            Module* newModule = nodeModule->create();
            if (newModule) {
                // set the nodeID of module
                newModule->node = ID;
                m_nodeModules[nodeModule->ID()][ID] = newModule;
            }
            else {
                m_errorString = nodeModule->errorString();
                return 0;
            }
        }
    }

    // init last env modules
    foreach(ModuleAdapter* envModule, uninitEnvModules) {
        Module* initModule = m_envModules[envModule->ID()];

        QList<ModuleID> dependencies = envModule->dependencies();
        QList<Module*> depModules;

        foreach(ModuleID moduleID, dependencies) {
            // depend by module of node, 1-to-many relationship
            if (m_moduleType[moduleID] != ModuleType_Environment)
                for (NodeID nodeID = 0; nodeID < nodesNum; nodeID++)
                    depModules += m_nodeModules[moduleID][nodeID];
            // depend by module of env 1-to-1 relationship
            else
                depModules += m_envModules[moduleID];
        }

        // init module
        envModule->init(initModule, depModules);
    }

    // init modules of nodes
    foreach(ModuleAdapter* nodeModule, m_nodeAdapters) {
        // for all nodes
        for (NodeID nodeID = 0; nodeID < nodesNum; nodeID++) {
            Module* initModule = m_nodeModules[nodeModule->ID()][nodeID];

            QList<ModuleID> dependencies = nodeModule->dependencies();
            QList<Module*> depModules;

            foreach(ModuleID moduleID, dependencies) {
                // depend by module of env 1-to-1 relationship
                if (m_moduleType[moduleID] == ModuleType_Environment)
                    depModules += m_envModules[moduleID];
                // depend by module of node 1-to-1 relationship, need the same nodeID
                else
                    depModules += m_nodeModules[moduleID][nodeID];
            }

            // init module
            nodeModule->init(initModule, depModules);
        }
    }

    return 1;
}

#endif
