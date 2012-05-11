/**
 *
 * Irtx.h - RTX interface
 *
 **/

#ifndef IRTX_H
#define IRTX_H

#include <QtCore>

#include "IHardware.h"

#include "types.h"
#include "nodeEvent.h"
#include "loggableEvent.h"
#include "processableEvent.h"

class Irtx : public IHardware
{
public:
    virtual void setTXPower(int power) = 0;
    virtual void setChannel(int newChannel) = 0;
    virtual void setPower(bool on) = 0;
    virtual void setCCAThreshold(int threshold) = 0;
    
    virtual void startTX(byteArray message) = 0;
    virtual void startTX(byteArray message, void (*handler)()) = 0;
    virtual void waitTXEnd() = 0;
    virtual bool CCA() = 0;

    virtual int TXPower() = 0;
    virtual int RXSensivity() const = 0;

    // virtual void addRXInterrupt()

    class SFD_RX_Up : public nodeEvent
// , public loggableEvent
    {
    public:
        SFD_RX_Up() { count++; }
        virtual QString eventName() const { return "SFD_RX_Up"; }
        virtual void record(QDataStream& stream) { stream << eventNode; }
        virtual void process();

        static quint64 count;
    };

    class SFD_RX_Down : public nodeEvent
// , public loggableEvent, public processableEvent
    {
    public:
        SFD_RX_Down() { count++; }

        byteArray message;
        int RSSI;
        InterruptHandler handler;

        static quint64 count;
        
        virtual QString eventName() const { return "SFD_RX_Down"; }
        virtual void record(QDataStream& stream) 
        {
            stream << eventNode << ((quint8)message.size());
            stream.writeRawData(message, message.size());
            stream << RSSI;
        }

        virtual void process();
        // {
        //     if (handler != NULL)
        //         handler();
        // }

    };

    class SFD_TX_Up : public nodeEvent
// , public loggableEvent, public processableEvent
    {
    public:
        SFD_TX_Up() { count++; }
        byteArray message;
        int TXPower;
        InterruptHandler handler;

        static quint64 count;

        virtual void process()
        {
            if (handler != NULL)
                handler();
        }

        virtual QString eventName() const { return "SFD_TX_Up"; }
        virtual void record(QDataStream& stream)
        {
            stream << eventNode << ((quint8)message.size());
            stream.writeRawData(message, message.size());
            stream << TXPower;
        }
    };

    class SFD_TX_Down : public nodeEvent
// , public loggableEvent
    {
    public:
        SFD_TX_Down() { count++; }

        static quint64 count;

        virtual QString eventName() const { return "SFD_TX_Down"; }
        virtual void record(QDataStream& stream) { stream << eventNode; }
        virtual void process();
        // {
        //     qDebug() << "in TX Down interrupt";
        //     qDebug() << "rtx state set RXON";
        // }
    };

    class Collision : public nodeEvent
    {
    public:
        Collision() { count++; }

        static quint64 count;


        // FIXME: нужен список NodeID
        virtual QString eventName() const { return "Collision"; }
        virtual void record(QDataStream& stream) { stream << eventNode; }
        virtual void process() {};
    };

    class CCATest : public nodeEvent
    {
    public:
        CCATest() { count++; }

        static quint64 count;

        int result;
        virtual QString eventName() const { return "CCATest"; }
        virtual void record(QDataStream& stream) { stream << eventNode << result; }
        virtual void process() {};
    };


};
Q_DECLARE_INTERFACE(Irtx,
                    "simulator.IRtx/0.1");

#endif // IRTX_H
