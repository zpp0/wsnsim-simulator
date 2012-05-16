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
        qDebug() << "in startTX node" << m_parentNode->ID();

        m_event->post(this, "SFD_TX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << m_TXPower);

        m_state = rtxState_TXON;
        qDebug() << "radio state set TXON";

        timeTXEnd = message.size() * 32;
        // qDebug() << "timeEnd set";

        m_channel->send(m_parentNode, message);

        m_event->post(this, "SFD_TX_Up", timeTXEnd,
                      QVariantList() << m_parentNode->ID());
    }
}

void RTX::startTX(byteArray message, void (*handler)())
{
    m_event->post(this, "SFD_TX_Up", 0,
                  QVariantList() << m_parentNode->ID() << message << m_TXPower);

    m_state = rtxState_TXON;
}

void RTX::waitTXEnd()
{
    // TODO:
    // if (Env::time < timeTXEnd)
    //     Env::time = timeTXEnd;

    m_state = rtxState_Free;
    qDebug() << "rtx state set Free";
}

bool RTX::CCA()
{
    bool state = m_channel->aroundPower(m_parentNode) < m_CCAThreshold;

    qDebug() << "in CCA test" << state;

    m_event->post(this, "CCATest", 0,
                  QVariantList() << m_parentNode->ID() << state);

    return state;
}

int RTX::TXPower()
{
    return m_TXPower;
}

int RTX::RXSensivity() const
{
    return m_RXSensivity;
}

void RTX::eventHandler(QString name, QVariantList params)
{
    // FIXME: ugly code
    if (name == "newMessage")
        // FIXME: static type checking
        newMessageEvent(params[1].toByteArray(), params[2].toDouble());
    if (name == "SFD_RX_Up")
        SFD_RX_Up_Event(params[1].toByteArray(), params[2].toDouble());
    if (name == "SFD_RX_Down")
        SFD_RX_Down_Event(params[1].toByteArray());
    if (name == "SFD_TX_Up")
        SFD_TX_Up_Event(params[1].toByteArray(), params[2].toDouble());
    if (name == "SFD_TX_Down")
        SFD_TX_Down_Event();
}

void RTX::newMessageEvent(byteArray message, double RSSI)
{
    if (m_state == rtxState_Free)
        m_event->post(this, "SFD_RX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << RSSI);

    else if ((m_state == rtxState_RXON) && (abs(RSSI - m_currentRX_RSSI) < 0.01)) {
        m_event->post(this, "message_dropped", 0,
                      QVariantList() << m_parentNode->ID());
        m_state = rtxState_Free;
        m_currentRX_RSSI = 0;
        m_event->post(this, "Collision", 0,
                      QVariantList() << m_parentNode->ID());
    }

    else if ((m_state == rtxState_RXON) && (RSSI > m_currentRX_RSSI)) {
        m_event->post(this, "message_dropped", 0,
                      QVariantList() << m_parentNode->ID());
        m_state = rtxState_Free;
        m_event->post(this, "SFD_RX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << RSSI);
    }
}

void RTX::SFD_RX_Up_Event(byteArray message, double RSSI)
{
    m_state = rtxState_RXON;
    m_currentRX_RSSI = RSSI;
    m_event->post(this, "SFD_RX_Down", message.length() * 32,
                  QVariantList() << m_parentNode->ID() << message);
}

void RTX::SFD_RX_Down_Event(byteArray message)
{
    // FIXME: ugly code
    // TODO: IEvent->post can return some value which can help to delete event from queue
    if (m_state == rtxState_RXON)
        m_event->post(this, "MessageReceived", 0,
                      QVariantList() << m_parentNode->ID() << message);
}

void RTX::SFD_TX_Up_Event(byteArray message, double TXPower)
{
    m_state = rtxState_TXON;
}

void RTX::SFD_TX_Down_Event()
{
    m_state = rtxState_Free;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(rtx, RTX);
QT_END_NAMESPACE
