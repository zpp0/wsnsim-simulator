/**
 *
 * Irtx.h - RTX interface
 *
 **/

#ifndef IRTX_H
#define IRTX_H

#include <QtCore>

#include "IHardware.h"

class Irtx : public IHardware, public QObject
{
    Q_OBJECT
public:
    Irtx()
    {
        interfaceInfo.name = "Irtx";

        interfaceInfo.events["SFD_RX_Up"]["NodeID"] = "NodeID";

        interfaceInfo.events["SFD_RX_Down"]["NodeID"] = "NodeID";
        interfaceInfo.events["SFD_RX_Down"]["message"] = "byteArray";
        interfaceInfo.events["SFD_RX_Down"]["RSSI"] = "double";

        interfaceInfo.events["SFD_TX_Up"]["NodeID"] = "NodeID";
        interfaceInfo.events["SFD_TX_Up"]["message"] = "byteArray";
        interfaceInfo.events["SFD_TX_Up"]["TXPower"] = "double";

        interfaceInfo.events["SFD_TX_Down"]["NodeID"] = "NodeID";

        interfaceInfo.events["Collision"]["NodeID"] = "NodeID";

        interfaceInfo.events["CCATest"]["NodeID"] = "NodeID";
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
Q_DECLARE_INTERFACE(Irtx,
                    "simulator.IRtx/0.1");

#endif // IRTX_H
