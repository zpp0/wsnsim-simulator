/**
 *
 * rtx.h
 *
 **/

#ifndef RTX_H
#define RTX_H

#include <QtCore>

#include "Irtx.h"

#include "IRadioChannel.h"
#include "INode.h"
#include "IEvent.h"

class RTX : public Irtx
{
    Q_OBJECT
    Q_INTERFACES(Irtx)

public:
    RTX()
    {
        moduleInfo.name = "RTX ";
        moduleInfo.version = "0.1";
        moduleInfo.description = "";
        moduleInfo.exportInterface = "Irtx";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IRadioChannel";
        moduleInfo.importInterfaces += "IEvent";

        moduleInfo.params["RXSensivity"] = "int";
        moduleInfo.params["TXPower"] = "int";
        moduleInfo.params["CCAThreshold"] = "int";

        moduleInfo.handledEvents += "newMessage";
        moduleInfo.handledEvents += "SFD_RX_Up";
        moduleInfo.handledEvents += "SFD_RX_Down";
        moduleInfo.handledEvents += "SFD_TX_Up";
        moduleInfo.handledEvents += "SFD_TX_Down";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void setTXPower(int power);
    /* virtual */ void setChannel(int newChannel);
    /* virtual */ void setPower(bool on);
    /* virtual */ void setCCAThreshold(int threshold);

    /* virtual */ void startTX(byteArray message);
    /* virtual */ void startTX(byteArray message, void (*handler)());
    /* virtual */ void waitTXEnd();
    /* virtual */ bool CCA();

    /* virtual */ int TXPower();
    /* virtual */ int RXSensivity() const;

    /* virtual */ void eventHandler(QString name, QVariantList params);

private:

    void newMessageEvent(byteArray message, double RSSI);
    void SFD_RX_Up_Event(byteArray message, double RSSI);
    void SFD_RX_Down_Event(byteArray message);
    void SFD_TX_Up_Event(byteArray message, double TXPower);
    void SFD_TX_Down_Event();

    IRadioChannel* m_channel;

    INode* m_parentNode;
    IEvent* m_event;

    int m_RXSensivity;
    int m_TXPower;
    int m_CCAThreshold;
    // int m_channel;

    VirtualTime timeTXEnd;

    double m_currentRX_RSSI;

    enum rtxState
    {
        rtxState_OFF,
        rtxState_Free,
        rtxState_RXON,
        rtxState_TXON
    };

    rtxState m_state;
};

#endif // RTX_H
