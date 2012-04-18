/**
 *
 * IHardware.h - node's Hardware interface
 *
 **/

#ifndef IHARDWARE_H
#define IHARDWARE_H

#include <QtCore>

#include "types.h"

#include "IModule.h"

class IHardware : public IModule
{
    // Q_OBJECT

public:
	virtual ~IHardware() {}
    
    virtual QString deviceName() const = 0;

    virtual QList<InterruptHandler> interrupts() = 0;
};

#endif // IHARDWARE_H
