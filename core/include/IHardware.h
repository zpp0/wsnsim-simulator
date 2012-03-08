/**
 *
 * IHardware.h - node's Hardware interface
 *
 **/

#ifndef IHARDWARE_H
#define IHARDWARE_H

#include <QtCore>

#include "types.h"

class IHardware
{
public:
	virtual ~IHardware() {}
    
    virtual QString deviceName() const = 0;

    virtual QList<interruptHandler> interrupts() = 0;
};

#endif // IHARDWARE_H
