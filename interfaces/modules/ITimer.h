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
#include "loggableEvent.h"
#include "processableEvent.h"

class ITimer : public IHardware 
{
    // Q_OBJECT

public:
    virtual VirtualTime getValue() = 0;
    virtual void setValue(VirtualTime newTime) = 0;
    virtual void start(VirtualTime timeout, void (*timerInterrupt)(void*, QString), void* object, QString type) = 0;
    virtual void stop(InterruptHandler timerInterrupt) = 0;
    
    class timerInterrupt : public nodeEvent
// , public loggableEvent, public processableEvent
    {
    public:
        timerInterrupt() { count++; }
        void (*handler)(void*, QString);

        void* object;

        QString type;

        static quint64 count;

        virtual QString eventName() const { return "timerInterrupt"; }
        // TODO: записать type
        virtual void record(QDataStream& stream) { stream << eventNode; }
        
        virtual void process()
        {
            if (handler != NULL)
                handler(object, type);
        }
        // TODO: add timer's ID
    };
};
Q_DECLARE_INTERFACE(ITimer,
                    "simulator.ITimer/0.1")

#endif // ITIMER_H
