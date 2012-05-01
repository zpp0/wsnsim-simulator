/**
 *
 * IHardware.h - node's Hardware interface
 *
 **/

#ifndef IHARDWARE_H
#define IHARDWARE_H

#include "types.h"

#include "IModule.h"

struct IHardware : public IModule
{
	virtual ~IHardware() {}
    
    virtual QString deviceName() const = 0;

    // virtual QList<InterruptHandler> interrupts() = 0;
};

#endif // IHARDWARE_H
