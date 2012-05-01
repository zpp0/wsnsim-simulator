/**
 *
 * File: ISimulator.h
 * Description: Simulator core interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ISIMULATOR_H
#define ISIMULATOR_H

#include "ICore.h"

struct ISimulator : public ICore
{
    ISimulator() { interfaceInfo.name = "ISimulator"; }

    virtual ICore* getCoreInterface(IModule* receiver, QString interfaceName) = 0;
    virtual QList<QString> getEvents() = 0;
    virtual void registerEventHandler(IModule* handler, QString eventName) = 0;
};

#endif // ISIMULATOR_H
