/**
 *
 * File: nodePHY.cpp
 * Description: low level node's software module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "nodePHY.h"

bool NodePHY::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    // TODO: template
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    m_timer = (ITimer*)isimulator->getNodeInterface(this, m_parentNode, "ITimer");
    m_csma = (ICSMA_CA*)isimulator->getNodeInterface(this, m_parentNode, "ICSMA_CA");
    m_rtx = (Irtx*)isimulator->getNodeInterface(this, m_parentNode, "Irtx");

    m_sendMessagePeriod = params["sendMessagePeriod"].toULong();
    
    return true;
}

void NodePHY::init()
{
    m_timer->start(m_sendMessagePeriod, "trySendMessage");
}

void NodePHY::trySendMessage()
{
    QByteArray message;
        
    char string[] = "hello";

    message += "0000"; // преамбула
    message += 0x7A; // SFD?
    message += strlen(string); // length
    message += string; // data

    m_csma->sendMessage(message);
}

void NodePHY::eventHandler(QString name, QVariantList params)
{
        // FIXME: ugly code
    if (name == "nodePowerUp")
        init();
    if (name == "timerInterrupt")
        if (params[1].toString() == "trySendMessage")
            trySendMessage();
    if (name == "CSMA_success" || name == "CSMA_fail")
        trySendMessage();
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(nodePHY, NodePHY);
QT_END_NAMESPACE
