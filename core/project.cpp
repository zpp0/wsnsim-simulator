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

ModuleType Project::getType(QString typeName)
{
    ModuleType type;
    if (typeName == "environment")
        type = ModuleType_Environment;
    else if (typeName == "hardware")
            type = ModuleType_Hardware;
    else if (typeName == "software")
        type = ModuleType_Software;
    else
        type = ModuleType_Undefined;

    return type;
}

#ifdef MODULES_ENABLED
int Project::loadModules()
{
    QDir modulesDir(QDir::currentPath() + "/modules");

    foreach(ModuleData moduleData, m_projectParams.modules) {

        QString moduleName = moduleData.moduleInfo["name"];
        QString fileName = moduleData.fileName;
        QString typeName = moduleData.moduleInfo["type"];

        // prepare module type
        ModuleType type = getType(typeName);
        if (type == ModuleType_Undefined) {
            m_errorString = "Wrong type " + typeName + " of module " + moduleName;
            return 0;
        }

        // prepare module params
        QVariantMap params;
        foreach(ModuleParam param, moduleData.params)
            params[param.name] = param.value;

        // prepare moduleID
        ModuleID ID = moduleData.moduleInfo["ID"].toInt();

        QList<ModuleDependence> dependencies;
        // prepare dependencies
        foreach(QString depName, moduleData.dependencies.keys()) {
            ModuleDependence dep;
            dep.name = depName;
            QString typeName = moduleData.dependencies[depName].first;
            dep.moduleID = moduleData.dependencies[depName].second;

            ModuleType type = getType(typeName);
            if (type == ModuleType_Undefined) {
                m_errorString = "Wrong type " + typeName + " of dependence " + depName;
                return 0;
            }
            dep.type = type;

            dependencies += dep;
        }
        
        Module module;
        module.name = moduleName;
        module.ID = ID;
        module.type = type;
        module.fileName = fileName;
        module.params = params;
        module.dependencies = dependencies;

        // create new adapter
        ModuleAdapter* loader;

        if (moduleData.moduleInfo["lang"] == "cpp") {
            // TODO: implement this
        }

#ifdef LUA_ENABLED
        else if (moduleData.moduleInfo["lang"] == "lua")
            loader = (ModuleAdapter*) new ModuleAdapterLua(module);
#endif

        if (loader == NULL) {
            m_errorString = "Can't load module " + moduleData.moduleInfo["name"];
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

int Project::createModules()
{
    m_nodesNum = m_projectParams.simulatorParams.nodes;

    // creating env modules
    foreach(ModuleAdapter* envModule, m_envAdapters) {

        ModuleInstanceID id = envModule->create();
        if (id != -1)
            m_envModules[envModule->ID()] = id;
        else {
            m_errorString = envModule->errorString();
            return 0;
        }
    }

    if (m_nodesNum.empty()) {
        m_errorString = "There are no registered nodes in simulator. Can't continue.";
        return 0;
    }

    // now we must have more than 0 registered nodes
    foreach(ModuleID moduleID, m_nodesNum.keys()) {
        quint16 nodesNum = m_nodesNum[moduleID];
        quint16 nodeID = 0;

        // creating nodes modules
        for (; nodeID < nodesNum; nodeID++) {
            foreach(ModuleAdapter* nodeModule, m_nodeAdapters) {

                ModuleInstanceID instanceID = nodeModule->create();
                if (instanceID != -1)
                    m_nodeModules[nodeModule->ID()][nodeID] = instanceID;
                else {
                    m_errorString = nodeModule->errorString();
                    return 0;
                }
            }
        }
    }

    return 1;
}

int Project::initModules()
{
    // init last env modules
    foreach(ModuleAdapter* envModule, m_envAdapters) {
        int success = envModule->init(m_envModules[envModule->ID()]);
        if (!success) {
            m_errorString = envModule->errorString();
            return 0;
        }
    }

    foreach(ModuleID moduleID, m_nodesNum.keys()) {
        quint16 nodesNum = m_nodesNum[moduleID];
        quint16 nodeID = 0;

        // init modules of nodes
        foreach(ModuleAdapter* nodeModule, m_nodeAdapters) {
            // for all nodes
            for (; nodeID < nodesNum; nodeID++) {
                // init module
                int success = nodeModule->init(m_nodeModules[nodeModule->ID()][nodeID]);
                if (!success) {
                    m_errorString = nodeModule->errorString();
                    return 0;
                }
            }
        }
    }

    return 1;
}

#endif
