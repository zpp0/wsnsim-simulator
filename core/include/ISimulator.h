/**
 *
 * File: ISimulator.h
 * Description: Simulator interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ISIMULATOR_H
#define ISIMULATOR_H

#include "ICore.h"
#include "IModule.h"

struct ISimulator : public ICore
{
	virtual ~ISimulator() {}

    // TODO: do events struct
    virtual void getEvents() = 0;
    virtual void registerEventHandler(IModule* module, QString eventName);
};

#endif // ISIMULATOR_H
















