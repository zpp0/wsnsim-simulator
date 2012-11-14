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

Project::Project(QString projectFileName)
{
    m_projectFileName = projectFileName;

    m_eventsParamsNames["int32"] = INT32_TYPE;
    m_eventsParamsNames["uint8"] = UINT8_TYPE;
    m_eventsParamsNames["uint16"] = UINT16_TYPE;
    m_eventsParamsNames["uint32"] = UINT32_TYPE;
    m_eventsParamsNames["uint64"] = UINT64_TYPE;
    m_eventsParamsNames["double"] = DOUBLE_TYPE;
    m_eventsParamsNames["string"] = STRING_TYPE;
    m_eventsParamsNames["ByteArray"] = BYTE_ARRAY_TYPE;
    m_eventsParamsNames["bool"] = BOOL_TYPE;
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

    m_nodesNum = m_projectParams.simulatorParams.nodes;

    if (m_nodesNum.empty()) {
        m_errorString = "There are no registered nodes in simulator. Can't continue.";
        return 0;
    }

    foreach (EventParams params, m_projectParams.events.systemEvents) {
        QString eventName = params.eventInfo["name"];
        ModuleID moduleID = params.eventInfo["moduleID"].toUInt();
        EventID eventID = params.eventInfo["ID"].toUInt();
        Simulator::registerEvent(eventName, moduleID, eventID);

        foreach (EventArgument arg, params.arguments) {
            QString name = arg["name"];
            quint16 ID = arg["ID"].toUInt();
            QString typeName = arg["type"];
            EventParamType type = m_eventsParamsNames.value(typeName, UNKNOWN_TYPE);
            if (type == UNKNOWN_TYPE) {
                m_errorString = "Wrong type " + typeName + " of event argument " + name;
                return 0;
            }
            Simulator::registerEventParam(eventName, moduleID, name, ID, type);
        }

    }

    return 1;
}

int Project::initLog()
{
    int ret = Log::init(m_projectParams.simulatorParams.logFile);
    if (ret == 0) {
        m_errorString = Log::errorString();
        return 0;
    }

    return 1;
}

int Project::initLua()
{
    LuaHost::open();
    return 1;
}

int Project::loadModules()
{
    QDir modulesDir(QDir::currentPath() + "/modules");

    foreach(ModuleData moduleData, m_projectParams.modules) {
        Module module(moduleData);

        if (!isValidModule(module))
            return 0;

        if (module.type == ModuleType_Environment)
            m_envModules += module;
        else
            m_nodeModules += module;
    }

    return 1;
}

int Project::createModules()
{
    // creating env modules
    foreach(Module envModule, m_envModules) {
        int ret = LuaHost::loadFile(envModule.fileName, envModule.name);
        if (!ret) {
            m_errorString = LuaHost::errorString();
            return 0;
        }

        ret = LuaHost::createModule(envModule.ID, 0, envModule.name);

        if (!ret)
            return 0;

        LuaHost::removeGlobalName(envModule.name);
    }

    // now we must have more than 0 registered nodes
    foreach(ModuleID moduleID, m_nodesNum.keys()) {
        quint16 nodesNum = m_nodesNum[moduleID];
        quint16 nodeID = 0;

        // creating nodes modules
        for (; nodeID < nodesNum; nodeID++) {

            // TODO: support fot heterogeneous nodes
            foreach(Module nodeModule, m_nodeModules) {

                int ret = LuaHost::createModule(nodeModule.ID, nodeID, nodeModule.name);
                
                if (!ret)
                    return 0;
            }
        }
    }

    return 1;
}

int Project::initModules()
{

    // init last env modules
    foreach(Module envModule, m_envModules) {
        int success = LuaHost::initModule(envModule.ID,
                                          0,
                                          envModule.type,
                                          envModule.params,
                                          envModule.dependencies);
        if (!success) {
            m_errorString = LuaHost::errorString();
            return 0;
        }
    }

    foreach(ModuleID moduleID, m_nodesNum.keys()) {
        quint16 nodesNum = m_nodesNum[moduleID];

        // init modules of nodes
        foreach(Module nodeModule, m_nodeModules) {
            quint16 nodeID = 0;

            // for all nodes
            for (; nodeID < nodesNum; nodeID++) {
                // init module
                int success = LuaHost::initModule(nodeModule.ID,
                                                  nodeID,
                                                  nodeModule.type,
                                                  nodeModule.params,
                                                  nodeModule.dependencies);
                if (!success) {
                    m_errorString = LuaHost::errorString();
                    return 0;
                }
            }
        }
        
    }

    return 1;
}

int Project::isValidModule(Module& module)
{
    // check possible errors
    if (module.type == ModuleType_Undefined) {
        m_errorString = "Wrong type of module " + module.name;
        return 0;
    }
    foreach (ModuleParameter param, module.params) {
        if (param.type == ModuleParamType_Undefined) {
            m_errorString = "Wrong type of module param " + param.name;
            return 0;
        }
    }
    foreach (ModuleDependence dep, module.dependencies) {
        if (dep.type == ModuleType_Undefined) {
            m_errorString = "Wrong type of dependence " + dep.name;
            return 0;
        }
    }
    
    // TODO: check modules ID for equality
    // TODO: check for existing of dependencies IDs
    
    return 1;
}
