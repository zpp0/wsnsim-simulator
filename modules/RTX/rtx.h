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

#include "node.h"

class RTX : public Irtx
{
    Q_OBJECT
	Q_INTERFACES(Irtx)

public:
    // RTX(Node* parentNode) : m_parentNode(parentNode) {};
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QString deviceName() const;
    virtual QList<InterruptHandler> interrupts();

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    // FIXME: DELETEA!!!1
    void setParentNode(Node* parentNode) { m_parentNode = parentNode; }

    virtual void setTXPower(int power);
    virtual void setChannel(int newChannel);
    virtual void setPower(bool on);
    virtual void setCCAThreshold(int threshold);
    
    virtual void startTX(byteArray message);
    virtual void startTX(byteArray message, void (*handler)());
    virtual void waitTXEnd();
    virtual bool CCA();

    virtual int TXPower();
    virtual int RXSensivity() const;

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
