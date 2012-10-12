/**
 *
 * File: project.cpp
 * Description: initialization of simulator's project
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "project.h"
#include "log.h"

Project::Project(QString projectFileName)
{
    m_projectFileName = projectFileName;
}

int Project::load(QString &errorString)
{
    QLibrary projectDataLibrary("./projectData");

    if (!projectDataLib.load()) {
        errorString = projectDataLibrary.errorString();
        return 0;
    }

    // FIXME: make it easy
    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad pd = (projectDataLoad) projectDataLib.resolve("load");

    m_projectParams = pd(m_projectFileName, &errorString);

    // if error, than return
    if (errorString != "")
        return 0;

    return 1;

    // TODO: check errors
}

int Project::initLog(QString &errorString)
{
    errorString = "";

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
