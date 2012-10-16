/**
 *
 * File: moduleAdapter_lua.h
 * Description: lua modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULEADAPTERLUA_H
#define MODULEADAPTERLUA_H

#include "moduleAdapter.h"

class ModuleAdapterLua : protected ModuleAdapter
{
public:
    ModuleAdapterLua(Module module);

    /* virtual */ int load();
    /* virtual */ ModuleInstanceID create();
    /* virtual */ int init(ModuleInstanceID moduleInstance);
    /* virtual */ QString errorString();

private:
    QString m_errorString;
    ModuleInstanceID m_ID;
    QList<ModuleInstanceID> m_modules;
};

#endif // MODULEADAPTERLUA_H


