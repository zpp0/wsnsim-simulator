/**
 *
 * File: project.cpp
 * Description: initialization of simulator's project
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QDir>
#include <QFile>
#include <QSettings>

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

    foreach(NodesData nodeData, m_projectParams.nodes) {
        NodeType type;
        QString name = nodeData.nodeType;
        foreach(NodeTypeData nodeType, m_projectParams.nodeTypes) {
            if (nodeType.name == name) {
                type.platform = nodeType.hardwareModules;
                type.application = nodeType.softwareModules;
                break;
            }
        }

        Nodes nodes;
        nodes.moduleID = nodeData.moduleID;
        nodes.nodeType = type;
        nodes.number = nodeData.nodesNumber;

        m_nodesNum += nodes;
    }

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

Module* Project::findNodeModule(ModuleID moduleID)
{
    foreach(const Module& module, m_nodeModules)
        if (module.ID == moduleID)
            return (Module*)&module;
    return NULL;
}

QString Project::getModulePath(QString fileName)
{
    QSettings settings("wsnsim", "simulator");
    QString modulesDirectory = settings.value("Modules/Directory").toString();
    QString file = QDir::currentPath() + modulesDirectory + fileName;
    return file;
}

int Project::createNodes(Nodes nodes, int nodesTotal)
{
    NodeType nodeType = nodes.nodeType;

    QList<QList<ModuleID> > modulesList;
    modulesList += nodeType.platform;
    modulesList += nodeType.application;

    foreach(QList<ModuleID> modules, modulesList) {
        foreach(ModuleID moduleID, modules) {
            Module* nodeModule = findNodeModule(moduleID);

            if (!nodeModule)
                return 0;

            int ret = LuaHost::loadFile(getModulePath(nodeModule->fileName), nodeModule->name);
            if (!ret) {
                m_errorString = LuaHost::errorString();
                return 0;
            }

            // creating nodes modules
            for (int i = 0; i < nodes.number; i++) {

                int ret = LuaHost::createModule(nodeModule->ID, i + nodesTotal, nodeModule->name);

                if (!ret) {
                    m_errorString = LuaHost::errorString();
                    return 0;
                }
            }

            LuaHost::removeGlobalName(nodeModule->name);
        }
    }

    return 1;
}

int Project::initNodes(Nodes nodes, int nodesTotal)
{
        // creating nodes modules
    for (int i = 0; i < nodes.number; i++) {
        NodeType nodeType = nodes.nodeType;

        QList<QList<ModuleID> > modulesList;
        modulesList += nodeType.platform;
        modulesList += nodeType.application;

        foreach(QList<ModuleID> modules, modulesList) {
            foreach(ModuleID moduleID, modules) {
                Module* nodeModule = findNodeModule(moduleID);
                if (!nodeModule)
                    return 0;

                // init module
                int success = LuaHost::initModule(nodeModule->ID,
                                                  i + nodesTotal,
                                                  nodeModule->type,
                                                  nodeModule->params,
                                                  nodeModule->dependencies);
                if (!success) {
                    m_errorString = LuaHost::errorString();
                    return 0;
                }
            }
        }
    }

    return 1;
}

int Project::createModules()
{
    // creating env modules
    foreach(Module envModule, m_envModules) {
        int ret = LuaHost::loadFile(getModulePath(envModule.fileName), envModule.name);
        if (!ret) {
            m_errorString = LuaHost::errorString();
            return 0;
        }

        ret = LuaHost::createModule(envModule.ID, 0, envModule.name);

        if (!ret) {
            m_errorString = LuaHost::errorString();
            return 0;
        }

        LuaHost::removeGlobalName(envModule.name);
    }

    quint16 nodesTotal = 0;

    foreach(Nodes nodes, m_nodesNum) {
        if (!createNodes(nodes, nodesTotal))
            return 0;
        nodesTotal += nodes.number;
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

    quint16 nodesTotal = 0;

    foreach(Nodes nodes, m_nodesNum) {
        if (!initNodes(nodes, nodesTotal))
            return 0;
        nodesTotal += nodes.number;
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
    foreach (ModuleDepend dep, module.dependencies) {
        if (dep.type == ModuleType_Undefined) {
            m_errorString = "Wrong type of dependence " + dep.name;
            return 0;
        }
    }

    // TODO: check modules ID for equality
    // TODO: check for existing of dependencies IDs

    return 1;
}
