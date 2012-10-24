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
#include "event.h"

class SimulatorAPI
{
public:
    VirtualTime globalTime();
    void postEvent(ModuleID author,
                   ModuleInstanceID authorID,
                   QString name,
                   VirtualTime time,
                   QList<EventParam> params);
};

#endif // SIMULATORAPI_H
