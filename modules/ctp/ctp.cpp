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
    quint8 messageType = message[0];

    QByteArray senderArr = message.mid(1, 19);
    quint64 sender = senderArr.toULong();
    // quint64 sender = message[1];
    // for (int i = 2; i < 9; i++)
    //     sender = (sender << 8) + message[i];

    // qDebug() << sender;

    QByteArray seqNumArr = message.mid(20, 1);
    quint8 seqNum = seqNumArr.toInt();

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
        QByteArray receiverArr = message.mid(21, 19);
        quint64 receiver = receiverArr.toULong();
        QByteArray valueArr = message.mid(40);
        double value = valueArr.toDouble();

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
    char arr[80];

    sprintf(arr, "%c%0llu%0u",
            CTPMessageType_Request,
            m_longAddr,
            seqNum);

    // qDebug() << "arr" << arr << "addr" << m_rtx->getLongAddr() << "seqnum" << seqNum;

    QByteArray message(arr);
    // QByteArray message;
    // message.append(CTPMessageType_Request);
    // message.append(QByteArray::number(m_rtx->getLongAddr(), 16));
    // message.append(seqNum);

    m_message = message;

    m_csma->sendMessage(message);

    m_event->post(this, "measureRequestSent", 0,
                      QVariantList() << m_parentNode->ID() << m_longAddr << seqNum);
}

void CTP::sendMeasureAck(double measuringValue, quint64 sender, quint64 receiver, quint8 seqNum)
{
    char arr[100];

    sprintf(arr, "%c%0llu%0u%0llu%f",
            CTPMessageType_Ack,
            sender,
            seqNum,
            receiver,
            measuringValue);

    // qDebug() << "arr" << arr;

    QByteArray message(arr);
    // QByteArray message;

    m_message = message;

    m_csma->sendMessage(message);

    m_event->post(this, "measureAckSent", 0,
                  QVariantList() << m_parentNode->ID() << receiver << sender << seqNum << measuringValue);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(ctp, CTP);
QT_END_NAMESPACE
