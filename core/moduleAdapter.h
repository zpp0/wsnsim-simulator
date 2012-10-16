/**
 *
 * File: moduleAdapter.h
 * Description: module adapter functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef MODULEADAPTER_H
#define MODULEADAPTER_H

#include "module.h"

class ModuleAdapter {
public:
    ModuleAdapter(Module module);

    virtual ~ModuleAdapter(){}
    virtual int load() = 0;
    virtual ModuleInstanceID create() = 0;
    virtual int init(ModuleInstanceID moduleInstance) = 0;
    virtual QString errorString() = 0;

    ModuleID ID() { return m_module.ID; }
    ModuleType moduleType() { return m_module.type; }
    ModuleDependencies dependencies() { return m_module.dependencies; }

protected:
    Module m_module;
};

#endif // MODULEADAPTER_H
