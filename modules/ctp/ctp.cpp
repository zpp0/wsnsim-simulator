/**
 *
 * File: ctp.cpp
 * Description: CTP module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "ctp.h"

bool CTP::moduleInit(ISimulator* isimulator,
                     QMap<QString, QString> params)
{

    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    qDebug() << m_parentNode;
    m_timer = (ITimer*)isimulator->getNodeInterface(this, m_parentNode, "ITimer");
    qDebug() << m_timer;
    m_csma = (ICSMA_CA*)isimulator->getNodeInterface(this, m_parentNode, "ICSMA_CA");
        qDebug() << m_csma;
    m_rtx = (Irtx*)isimulator->getNodeInterface(this, m_parentNode, "Irtx");
            qDebug() << m_rtx;
    m_sensor = (IAsyncSensor*)isimulator->getNodeInterface(this, m_parentNode, "IAsyncSensor");
                qDebug() << m_sensor;

    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");
                    qDebug() << m_event;

    m_wakeUpPeriod = params["messageSendingPeriod"].toULong();

    m_seqNum = 0;

    m_needSendAck = false;

    return true;
}

void CTP::eventHandler(QString name, QVariantList params)
{
    if (name == "timerInterrupt")
        if (params[1].toString() == "messageSending") {
            sendMeasureReq(++m_seqNum);
            m_timer->start(m_wakeUpPeriod, "messageSending");
        }

    if (name == "measuring_get_result") {
        m_currentMeasuringValue = params[2].toDouble();
        qDebug() << "measuring_get_result" << "state" << m_rtx->state();
        if (m_rtx->state() != rtxState_Free)
            m_needSendAck = true;
        else
            sendMeasureAck(m_currentMeasuringValue, m_longAddr, m_parent, m_seqNum);
    }

    if (name == "nodePowerUp") {
        m_longAddr = m_rtx->getLongAddr();
        if (m_parentNode->ID() == 0)
            m_timer->start(m_wakeUpPeriod, "messageSending");
    }

    if (name == "MessageReceived") {
        qDebug() << "123123";
        recognizeMessage(params[1].toByteArray());
    }

    if (name == "MessageSent") {
        qDebug() << "MessageSent" << m_needSendAck;
        if (m_parentNode->ID() != 0) {
            if (m_needSendAck == true) {
                m_needSendAck = false;
                qDebug() << "node" << QString::number(m_parentNode->ID()) << "sending ack with seqnum" << QString::number(m_seqNum);
                sendMeasureAck(m_currentMeasuringValue, m_longAddr, m_parent, m_seqNum);
            }
        }
    }

    if (name == "CSMA_fail")
        m_csma->sendMessage(m_message);
}

void CTP::recognizeMessage(byteArray message)
{
    qDebug() << "recognise message" << message;

    for (int i = 0; i < message.size(); i++)
        qDebug() << QString::number(message[i]);

    quint8 messageType = message[1];

    // QByteArray senderArr = message.mid(1, 19);
    // quint64 sender = senderArr.toULong();
    quint64 sender = message[2];
    // for (int i = 2; i < 9; i++)
    //     sender = (sender << 8) + message[i];

    // qDebug() << sender;

    // QByteArray seqNumArr = message.mid(20, 1);
    // quint8 seqNum = seqNumArr.toInt();
    quint8 seqNum = message[3];

    qDebug() << "etetet" << sender << seqNum << messageType;

    switch (messageType) {

    case CTPMessageType_Request:

        if (m_parentNode->ID() == 0)
            return;

        if (m_seqNums.contains(seqNum))
            return;

        m_event->post(this, "measureRequestReceived", 0,
                      QVariantList() << m_parentNode->ID() << sender << seqNum);

        m_seqNum = seqNum;
        qDebug() << "node" << QString::number(m_parentNode->ID()) << "set seqnum as" << QString::number(m_seqNum);
        m_seqNums += seqNum;
        if (m_seqNums.size() > 5)
            m_seqNums.pop_front();

        m_parent = sender;

        sendMeasureReq(seqNum);

        m_sensor->measure();

        break;

    case CTPMessageType_Ack:
    {
        // QByteArray receiverArr = message.mid(21, 19);
        // quint64 receiver = receiverArr.toULong();
        quint64 receiver = message[4];
        // QByteArray valueArr = message.mid(40);
        // double value = valueArr.toDouble();
        double value = message[5];

        qDebug() << "tetete" << receiver << value;

        if (receiver == m_longAddr) {
            m_event->post(this, "measureAckReceived", 0,
                          QVariantList() << m_parentNode->ID() << sender <<  seqNum << value);

            if (m_parentNode->ID() != 0)
                sendMeasureAck(value, sender, m_parent, seqNum);
            else
                m_event->post(this, "measureSentUART", 0,
                              QVariantList() << m_parentNode->ID() << sender << seqNum << value);
        }

        break;
    }

    }
}

void CTP::sendMeasureReq(quint8 seqNum)
{
    // char arr[80];

    // sprintf(arr, "%c%llu%u",
    //         CTPMessageType_Request,
    //         m_longAddr,
    //         seqNum);

    // char arr[3] = { CTPMessageType_Request, (quint8)m_longAddr, seqNum };
    // qDebug() << "arr" << arr << "addr" << m_rtx->getLongAddr() << "seqnum" << seqNum;


    // QByteArray message = QByteArray::fromRawData(arr, 3);
    // QByteArray message(arr);
    QByteArray message;

    message.append(CTPMessageType_Request);
    message.append(m_longAddr);
    // message.append(0);
    message.append(seqNum);

    qDebug() << "sending message" << message;

    for (int i = 0; i < message.size(); i++)
        qDebug() << QString::number(message[i]);

    m_message = message;

    m_csma->sendMessage(message);

    m_event->post(this, "measureRequestSent", 0,
                      QVariantList() << m_parentNode->ID() << m_longAddr << seqNum);
}

void CTP::sendMeasureAck(double measuringValue, quint64 sender, quint64 receiver, quint8 seqNum)
{
    // char arr[100];
    // char arr[5] = { CTPMessageType_Ack, (quint8)sender, seqNum, (quint8)receiver, (quint8)measuringValue };

    // sprintf(arr, "%c%llu%u%llu%f",
    //         CTPMessageType_Ack,
    //         sender,
    //         seqNum,
    //         receiver,
    //         measuringValue);

    // qDebug() << "arr" << arr;

    // QByteArray message = QByteArray::fromRawData(arr, strlen(arr));
    // QByteArray message = QByteArray::fromRawData(arr, 5);
    QByteArray message;
    message += CTPMessageType_Ack;
    message += (quint8)sender;
    message += seqNum;
    message += (quint8)receiver;
    message += (quint8)measuringValue;

    qDebug() << "ack message" << message;

    for (int i = 0; i < message.size(); i++)
        qDebug() << QString::number(message[i]);


    // QByteArray message(arr);
    // QByteArray message;

    m_message = message;

    m_csma->sendMessage(message);

    m_event->post(this, "measureAckSent", 0,
                  QVariantList() << m_parentNode->ID() << receiver << sender << seqNum << measuringValue);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(ctp, CTP);
QT_END_NAMESPACE
