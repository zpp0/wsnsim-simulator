/**
 *
 * File: project.h
 * Description: initialization of simulator's project
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QtCore>

#include "types.h"
#include "projectParams.h"

class Project {
public:
    Project(QString projectFileName);

    int load(QString &errorString);

    int initLog(QString &errorString);

#ifdef MODULES_ENABLED

#ifdef LUA_ENABLED
    void initLua();
#endif

    void loadModules();
    void createModules();
    void initModules();

#endif

private:
    QString m_projectFileName;
    ProjectParams m_projectParams;
    // QList<moduleLoader*> m_loaders;
    // QList<SimulatorModule*> m_modules;

    // QList<SimulatorModule*> m_uninitialisedModules;
};
