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

class RTX : public QObject, public Irtx
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    RTX()
    {
        moduleInfo.name = "TRX";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Приемник-передатчик стандарта IEEE802.15.4";
        moduleInfo.exportInterface = "Irtx";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IRadioChannel";
        moduleInfo.importInterfaces += "IEvent";

        moduleInfo.params["RXSensivity"] = "int";
        moduleInfo.params["TXPower"] = "int";
        moduleInfo.params["CCAThreshold"] = "int";

        moduleInfo.paramDescription["RXSensivity"] = "Чувствительность приемника в dBm (int)";
        moduleInfo.paramDescription["TXPower"] = "Мощность передатчика в dBm (int)";
        moduleInfo.paramDescription["CCAThreshold"] = "Пороговая мощность CCA в dBm (int)";

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

    /* virtual */ quint64 getLongAddr();

    /* virtual */ int TXPower();
    /* virtual */ int RXSensivity() const;

    /* virtual */ rtxState state();

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

    quint64 m_longAddr;

    rtxState m_state;

    bool m_uglycrap;
};

#endif // RTX_H
