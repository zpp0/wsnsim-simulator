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

class ITimer : public IHardware
{
public:
    virtual ~ITimer() {}
    ITimer()
    {
        interfaceInfo.name = "ITimer";
        interfaceInfo.events["timerInterrupt"]["NodeID"] = "NodeID";
        interfaceInfo.events["timerInterrupt"]["Type"] = "String";
    }

    virtual VirtualTime getValue() = 0;
    virtual void setValue(VirtualTime newTime) = 0;
    virtual void start(VirtualTime timeout, QString type) = 0;
    virtual void stop(QString type) = 0;
};

#endif // ITIMER_H
