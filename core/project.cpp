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

        ModuleInitData moduleInitData;
        moduleInitData.name = module.moduleInfo["name"];
        moduleInitData.ID = module.moduleInfo["ID"].toInt();
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

        if (loader->load())
            m_adapters += loader;
        else {
            m_errorString = loader->errorString();
            return 0;
        }
    }

    return 1;
}

int Project::createModules()
{
    // QList<ModuleData*> envModules;
    // foreach(ModuleData moduleData, m_projectParams.modules)
    //     if (moduleData.moduleInfo["type"] == "environment")
    //         envModules += &moduleData;

    // Module* scene;
    // foreach(ModuleData* envModule, envModules) {
    //     ModuleAdapter* adapter = m_moduleAdapters[envModule];
    //     Module* newModule = adapter->create();
    //     if (newModule) {
    //         // set module ID
    //         // newModule->ID = envModule.moduleInfo["ID"].toInt();
    //         // m_modules.insert(newModule, adapter);
    //         // if (
    //         // modules += newModule;
    //     }
    //     else {
    //         m_errorString = adapter->errorString();
    //         return 0;
    //     }
    // }

    return 1;
}

#endif
