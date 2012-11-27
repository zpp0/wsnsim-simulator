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

#include "virtualTime.h"
#include "projectParams.h"
#include "module.h"
#include "event.h"
#include "nodes.h"

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

    Module* findNodeModule(ModuleID moduleID);

    struct Nodes {
        ModuleID moduleID;
        NodeType nodeType;
        quint16 number;
    };

    int createNodes(Nodes nodes, int nodesTotal);
    int initNodes(Nodes nodes, int nodesTotal);

    QString getModulePath(QString fileName);

    QString m_projectFileName;
    ProjectParams m_projectParams;
    QString m_errorString;
    QMap<QString, EventParamType> m_eventsParamsNames;

    QList<Module> m_envModules;
    QList<Module> m_nodeModules;

    QList<Nodes> m_nodesNum;
};

#endif // PROJECT_H
