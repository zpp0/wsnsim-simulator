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
        QList<QString> dependence;
        dependence += "INode";
        dependence += "IRadioChannel";
        dependence += "IEvent";
        moduleInfo.importInterfaces = dependence;
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

    bool clearChannel();

    enum rtxState
    { 
        rtxState_OFF,
        rtxState_Free,
        rtxState_RXON, 
        rtxState_TXON
    };


    // TODO: move to private
    rtxState m_state;

private:

    IRadioChannel* m_channel;

    Node* m_parentNode;
    
    int m_RXSensivity;
    int m_TXPower;
    int m_CCAThreshold;
    // int m_channel;

    VirtualTime timeTXEnd;

};
// Q_EXPORT_PLUGIN(RTX);

#endif // RTX_H
