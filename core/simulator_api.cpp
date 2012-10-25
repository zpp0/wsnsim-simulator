/**
 *
 * File: simulator_api.cpp
 * Description: Simulator API
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "simulator_api.h"
#include "simulator.h"
#include "event.h"

VirtualTime SimulatorAPI::globalTime()
{
    return Simulator::globalTime();
}

void SimulatorAPI::postEvent(ModuleID author, QString name, VirtualTime delay, QVector<EventParam> params)
{
    Simulator::postEvent(author, name, delay, params);
};







