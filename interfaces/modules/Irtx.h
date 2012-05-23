/**
 *
 * Irtx.h - RTX interface
 *
 **/

#ifndef IRTX_H
#define IRTX_H

#include <QtCore>

#include "IHardware.h"

class Irtx : public IHardware
{
public:
    ~Irtx() {}
    Irtx()
    {
        interfaceInfo.name = "Irtx";

        interfaceInfo.events["SFD_RX_Up"]["NodeID"] = "uint16";
        interfaceInfo.events["SFD_RX_Up"]["message"] = "byteArray";
        interfaceInfo.events["SFD_RX_Up"]["RSSI"] = "double";

        interfaceInfo.events["SFD_RX_Down"]["NodeID"] = "uint16";
        interfaceInfo.events["SFD_RX_Down"]["message"] = "byteArray";

        interfaceInfo.events["SFD_TX_Up"]["NodeID"] = "uint16";
        interfaceInfo.events["SFD_TX_Up"]["message"] = "byteArray";
        interfaceInfo.events["SFD_TX_Up"]["TXPower"] = "double";

        interfaceInfo.events["SFD_TX_Down"]["NodeID"] = "uint16";

        interfaceInfo.events["Collision"]["NodeID"] = "uint16";

        interfaceInfo.events["CCATest"]["NodeID"] = "uint16";
        interfaceInfo.events["CCATest"]["State"] = "uint8";

        interfaceInfo.events["message_dropped"]["NodeID"] = "uint16";
    }

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

};

#endif // IRTX_H
