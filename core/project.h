/**
 *
 * File: project.h
 * Description: initialization of simulator's project
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef PROJECT_H
#define PROJECT_H

#include <QtCore>

#include "types.h"
#include "projectParams.h"
#include "module.h"
#include "event.h"

class Project {
public:
    Project(QString projectFileName);

    int load();

    int initSimulator();
    int initLog();
    int initLua();

    int loadModules();
    int createModules();
    int initModules();

    QString errorString();

private:
    int isValidModule(Module& module);

    QString m_projectFileName;
    ProjectParams m_projectParams;
    QString m_errorString;
    QMap<QString, EventParamType> m_eventsParamsNames;

    QList<Module> m_envModules;
    QList<Module> m_nodeModules;

    QHash<ModuleID, ModuleType> m_moduleType;

    QMap<ModuleID, NodeID> m_nodesNum;

    QHash<ModuleID, QHash<NodeID, ModuleInstanceID> > m_nodeModulesNum;
};

#endif // PROJECT_H
