/**
 *
 * rtx.cpp
 *
 **/

#include "rtx.h"

bool RTX::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    m_channel = (IRadioChannel*)isimulator->getEnvInterface(this, "IRadioChannel");
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");

    m_state = rtxState_Free;

    int RXSensivity;
    int TXPower;
    int CCAThreshold;

    RXSensivity = params["RXSensivity"].toInt();
    TXPower = params["TXPower"].toInt();
    CCAThreshold = params["CCAThreshold"].toInt();

    qDebug("RTX params gotten:");
    qDebug("RXSensivity: %i TXPower: %i CCAThreshold: %i",
           RXSensivity, TXPower, CCAThreshold);

    m_RXSensivity = RXSensivity;
    m_TXPower = TXPower;
    m_CCAThreshold = CCAThreshold;

    return true;
}

void RTX::setTXPower(int power)
{
    // TODO: do
}

void RTX::setChannel(int newChannel)
{
    // TODO: do
}

void RTX::setPower(bool on)
{
    if (on && m_state == rtxState_OFF)
        m_state = rtxState_RXON;
    else if (!on && m_state == rtxState_RXON)
        m_state = rtxState_OFF;
}

void RTX::setCCAThreshold(int threshold)
{
    // TODO: do
}

void RTX::startTX(byteArray message)
{
    if (m_state == rtxState_Free) {
        qDebug() << "in startTX node" << m_parentNode->ID;

        SFD_TX_Up* event = new SFD_TX_Up();
        qDebug() << "event count" << event::count;
        // qDebug() << "event created";
        event->time = Env::time;
        event->recordable = true;
        // qDebug() << "event time set";
        // qDebug() << "parent Node" << m_parentNode;
        event->eventNode = m_parentNode->ID;
        // qDebug() << "event ID set";
        event->message = message;
        // qDebug() << "event message set";
        event->TXPower = m_TXPower;
        // qDebug() << "event TXPow set";
        event->handler = NULL;
        // qDebug() << "event handler set";

        Env::queue.insert(event);
        // qDebug() << "event insert in queue";

        m_state = rtxState_TXON;
        qDebug() << "radio state set TXON";
        // FIXME: 32 or else?
        timeTXEnd = Env::time + message.size() * 32;
        // qDebug() << "timeEnd set";

        m_channel->send(m_parentNode, message);

        SFD_TX_Down* eventDown = new SFD_TX_Down();
        qDebug() << "event count" << event::count;
        // qDebug() << "event created";
        eventDown->time = timeTXEnd;
        // qDebug() << "event time set";
        eventDown->eventNode = m_parentNode->ID;
        // qDebug() << "event ID set";
        // eventDown->message = message;
        // qDebug() << "event message set";
        // eventDown->TXPower = m_TXPower;
        // qDebug() << "event TXPow set";
        // eventDown->handler = NULL;
        // qDebug() << "event handler set";

        Env::queue.insert(eventDown);
        // qDebug() << "event TXDown insert in queue";
    }
}

void RTX::startTX(byteArray message, void (*handler)())
{
    SFD_TX_Up* event = new SFD_TX_Up();
    qDebug() << "event count" << event::count;
    event->time = Env::time;
    event->eventNode = m_parentNode->ID;
    event->message = message;
    event->TXPower = m_TXPower;
    event->handler = handler;

    Env::queue.insert(event);

    m_state = rtxState_TXON;
}

void RTX::waitTXEnd()
{
    if (Env::time < timeTXEnd)
        Env::time = timeTXEnd;

    m_state = rtxState_Free;
    qDebug() << "rtx state set Free";
}

bool RTX::CCA()
{

    // FIXME: написать нормально

    // if (m_channel->aroundPower(m_parentNode) < m_CCAThreshold)
    bool result = clearChannel();

    qDebug() << "in CCA test" << result;

    CCATest* event = new CCATest();
    qDebug() << "event count" << event::count;
    event->time = Env::time;
    event->eventNode = m_parentNode->ID;
    event->result = result;

    // Env::queue.insert(event);

    log::writeLog(event);

    delete event;

    return result;
}

int RTX::TXPower()
{
    return m_TXPower;
}

int RTX::RXSensivity() const
{
    return m_RXSensivity;
}
