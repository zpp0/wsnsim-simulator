/**
 *
 * File: simulator_api.h
 * Description: Simulator API
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef SIMULATORAPI_H
#define SIMULATORAPI_H

#include "types.h"
#include "module.h"

class SimulatorAPI
{
public:
    VirtualTime globalTime();
    void postEvent(ModuleID author, QString name, VirtualTime time, QVariantList params);
};

#endif // SIMULATORAPI_H
