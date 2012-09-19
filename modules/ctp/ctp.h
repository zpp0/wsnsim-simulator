/**
 *
 * File: ctp.h
 * Description: command transfer protocol module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef CTP_H
#define CTP_H

#include <QtCore>

#include "ICTP.h"

#include "ICSMA_CA.h"
#include "ITimer.h"
#include "INode.h"
#include "IAsyncSensor.h"
#include "Irtx.h"
#include "IEvent.h"

class CTP : public QObject, public ICTP
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    CTP()
    {
        moduleInfo.name = "CTP";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Command transfer protocol";

        moduleInfo.exportInterface = "ICTP";

        moduleInfo.params["messageSendingPeriod"] = "quint64";

        moduleInfo.paramDescription["messageSendingPeriod"] = "Период отправки сообщений в мкс (uint64)";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "ITimer";
        moduleInfo.importInterfaces += "Irtx";
        moduleInfo.importInterfaces += "IAsyncSensor";
        moduleInfo.importInterfaces += "ICSMA_CA";
        moduleInfo.importInterfaces += "IEvent";

        moduleInfo.handledEvents += "CSMA_fail";
        moduleInfo.handledEvents += "MessageSent";
        moduleInfo.handledEvents += "MessageReceived";
        moduleInfo.handledEvents += "nodePowerUp";
        moduleInfo.handledEvents += "timerInterrupt";
        moduleInfo.handledEvents += "gotMeasuringResult";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void eventHandler(QString name, QVariantList params);

private:

    enum CTPMessageType {
        CTPMessageType_Request = 1,
        CTPMessageType_Ack,
    };

    void recognizeMessage(byteArray message);
    void sendMeasureReq(quint8 seqNum);
    void sendMeasureAck(double measuringValue, quint64 sender, quint64 receiver, quint8 seqNum);

    QVector<quint8> m_seqNums;
    quint8 m_seqNum;

    VirtualTime m_wakeUpPeriod;

    quint64 m_parent;

    quint64 m_longAddr;

    bool m_needSendAck;
    double m_currentMeasuringValue;

    QByteArray m_message;

    INode* m_parentNode;
    ITimer* m_timer;
    IAsyncSensor* m_sensor;
    ICSMA_CA* m_csma;
    Irtx* m_rtx;
    IEvent* m_event;
};

#endif // CTP_H
