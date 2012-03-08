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
#include "recordedEvent.h"
#include "processedEvent.h"

class Irtx : public IHardware
{
public:
    virtual void setTXPower(int power) = 0;
    virtual void setChannel(int newChannel) = 0;
    virtual void setPower(bool on) = 0;
    virtual void setCCAThreshold(int threshold) = 0;
    
    virtual void startTX(byteArray& message) = 0;
    virtual void startTX(byteArray& message, void (*handler)()) = 0;
    virtual void waitTXEnd() = 0;
    virtual bool CCA() = 0;

    virtual int TXPower() = 0;
    virtual int RXSensivity() const = 0;

    class SFD_RX_Up : public nodeEvent, public recordedEvent, public processedEvent { };

    class SFD_RX_Down : public nodeEvent, public recordedEvent, public processedEvent
    {
        byteArray message;
        int RSSI;
    };

    class SFD_TX_Up : public nodeEvent, public recordedEvent, public processedEvent
    {
        byteArray message;
        int TXPower;
    };

    class SFD_TX_Down : public nodeEvent, public recordedEvent, public processedEvent { };

};

#endif // IRTX_H
