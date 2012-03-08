/**
 *
 * ITimer.h - RTX interface
 *
 **/

#ifndef ITIMER_H
#define ITIMER_H

#include <QtCore>

#include "IHardware.h"

#include "types.h"
#include "nodeEvent.h"
#include "recordedEvent.h"
#include "processedEvent.h"

class ITimer : public IHardware
{
public:

    virtual virtualTime getValue() = 0;
    virtual void setValue(virtualTime newTime) = 0;
    virtual void start(virtualTime timeout, interruptHandler timerInterrupt);
    virtual void stop(interruptHandler timerInterrupt);
    
    class timerInterrupt : public nodeEvent, public recordedEvent, public processedEvent 
    {
        // TODO: add timer's ID
    };
};

#endif // ITIMER_H
