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

    foreach (EventParams params, m_projectParams.events.systemEvents) {
        QString eventName = params.eventInfo["name"];
        ModuleID moduleID = params.eventInfo["moduleID"].toUInt();
        EventID eventID = params.eventInfo["ID"].toUInt();
        Simulator::registerEvent(eventName, moduleID, eventID);

        foreach (EventArgument arg, params.arguments) {
            QString name = arg["name"];
            quint16 ID = arg["ID"].toUInt();
            QString typeName = arg["type"];
            EventParamType type = getEventParamType(typeName);
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

    foreach(ModuleData moduleData, m_projectParams.modules) {

        QString moduleName = moduleData.moduleInfo["name"];
        QString fileName = moduleData.fileName;
        QString typeName = moduleData.moduleInfo["type"];

        // prepare module type
        ModuleType type = getModuleType(typeName);
        if (type == ModuleType_Undefined) {
            m_errorString = "Wrong type " + typeName + " of module " + moduleName;
            return 0;
        }

        // prepare module params
        QList<ModuleParameter> params;
        foreach(ModuleParam param, moduleData.params) {
            ModuleParameter parameter;

            parameter.name = param.name;

            ModuleParamType type = getModuleParamType(param.type);
            if (type == ModuleParamType_Undefined) {
                m_errorString = "Wrong type " + typeName + " of module param " + param.name;
                return 0;
            }

            parameter.type = type;
            parameter.value = param.value;

            params += parameter;
        }

        // prepare moduleID
        ModuleID ID = moduleData.moduleInfo["ID"].toInt();

        // prepare dependencies
        QList<ModuleDependence> dependencies;

        foreach(QString depName, moduleData.dependencies.keys()) {
            ModuleDependence dep;
            dep.name = depName;
            QString typeName = moduleData.dependencies[depName].first;
            dep.moduleID = moduleData.dependencies[depName].second;

            ModuleType type = getModuleType(typeName);
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

        // init modules of nodes
        foreach(ModuleAdapter* nodeModule, m_nodeAdapters) {
            quint16 nodeID = 0;

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

ModuleType Project::getModuleType(QString typeName)
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

ModuleParamType Project::getModuleParamType(QString typeName)
{
    ModuleParamType type;
    if (typeName == "int")
        type = ModuleParamType_INT;
    else if (typeName == "uint8")
        type = ModuleParamType_UINT8;
    else if (typeName == "uint16")
        type = ModuleParamType_UINT16;
    else if (typeName == "uint32")
        type = ModuleParamType_UINT32;
    else if (typeName == "uint64")
        type = ModuleParamType_UINT64;
    else if (typeName == "double")
        type = ModuleParamType_DOUBLE;
    else if (typeName == "string")
        type = ModuleParamType_STRING;
    else if (typeName == "nodes")
        type = ModuleParamType_NODES;
    else
        type = ModuleParamType_Undefined;

    return type;
}

EventParamType Project::getEventParamType(QString typeName)
{
    EventParamType type;
    if (typeName == "int32")
        type = INT32_TYPE;
    else if (typeName == "uint8")
        type = UINT8_TYPE;
    else if (typeName == "uint16")
        type = UINT16_TYPE;
    else if (typeName == "uint32")
        type = UINT32_TYPE;
    else if (typeName == "uint64")
        type = UINT64_TYPE;
    else if (typeName == "double")
        type = DOUBLE_TYPE;
    else if (typeName == "string")
        type = STRING_TYPE;
    else if (typeName == "ByteArray")
        type = BYTE_ARRAY_TYPE;
    else if (typeName == "bool")
        type = BOOL_TYPE;

    else
        type = UNKNOWN_TYPE;

    return type;    
}
