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
#include "modules.h"

#define MODULES_ENABLED
#define LUA_ENABLED

class Project {
public:
    Project(QString projectFileName);

    int load();

    int initSimulator();
    int initLog();

#ifdef MODULES_ENABLED

#ifdef LUA_ENABLED
    int initLua();
#endif

    int loadModules();
    void createModules();
    void initModules();

#endif

    QString errorString();

private:
    QString m_projectFileName;
    ProjectParams m_projectParams;
    QString m_errorString;
    // QList<moduleLoader*> m_loaders;
    QMap<ModuleData*, ModuleAdapter*> m_moduleAdapters;
};

#endif // PROJECT_H
