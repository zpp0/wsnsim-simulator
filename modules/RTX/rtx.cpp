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
    bool useIDasMACAddr;

    RXSensivity = params["RXSensivity"].toInt();
    TXPower = params["TXPower"].toInt();
    CCAThreshold = params["CCAThreshold"].toInt();
    useIDasMACAddr = params["Use node ID as longAddr"].toInt();

    qDebug("RTX params gotten:");
    qDebug("RXSensivity: %i TXPower: %i CCAThreshold: %i",
           RXSensivity, TXPower, CCAThreshold);

    m_RXSensivity = RXSensivity;
    m_TXPower = TXPower;
    m_CCAThreshold = CCAThreshold;

    if (useIDasMACAddr)
        m_longAddr = m_parentNode->ID();

    else {
        m_longAddr = qrand();
        m_longAddr = (m_longAddr << 32) + qrand();
    }
    qDebug() << "longAddr" << m_longAddr;

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
        m_state = rtxState_Free;
    // else if (!on && m_state == rtxState_RXON)
    else if (!on)
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

        message.prepend(message.size());
        // message.prepend(0x7A);
        // char preambula[] = "0000"; //{0, 0, 0, 0};
        // message.prepend(preambula);

        qDebug() << "TX sending message" << message;
        for (int i = 0; i < message.size(); i++)
            qDebug() << QString::number(message[i]);

        m_event->post(this, "SFD_TX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << m_TXPower);

        m_state = rtxState_TXON;
        qDebug() << "radio state set TXON";

        timeTXEnd = (message.size() + 5) * 32;
        // qDebug() << "timeEnd set";

        qDebug() << "TX message" << message;

        m_channel->send(m_parentNode, message);

        m_event->post(this, "SFD_TX_Down", timeTXEnd,
                      QVariantList() << m_parentNode->ID());
    }
    else
        qDebug() << "not in startTX node" << m_parentNode->ID();
}

void RTX::startTX(byteArray message, void (*handler)())
{

    // message.prepend(message.size());
    // message.prepend(0x7A);
    // char preambula[] = "0000"; //{0, 0, 0, 0};
    // message.prepend(preambula);

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
    // bool state = m_channel->aroundPower(m_parentNode) < m_CCAThreshold;
    bool state = m_channel->aroundPower(m_parentNode) == 1;

    qDebug() << "in CCA test" << state << "on node" << m_parentNode->ID();

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
        if (m_state != rtxState_TXON)
            SFD_RX_Up_Event(params[1].toByteArray(), params[2].toDouble());
    if (name == "SFD_RX_Down")
        if (m_state == rtxState_RXON)
            SFD_RX_Down_Event(params[1].toByteArray());
    if (name == "SFD_TX_Up")
        if (m_state == rtxState_Free)
            SFD_TX_Up_Event(params[1].toByteArray(), params[2].toDouble());
    if (name == "SFD_TX_Down")
        if (m_state == rtxState_TXON)
            SFD_TX_Down_Event();
}

void RTX::newMessageEvent(byteArray message, double RSSI)
{
    if (m_state == rtxState_Free) {
        m_event->post(this, "SFD_RX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << RSSI);
        qDebug() << "120912" << "node" << m_parentNode->ID() << "get new message";
    }

    // else if (m_state == rtxState_RXON) {
    //     m_state = rtxState_Free;
    //     m_currentRX_RSSI = 0;
    //     m_event->post(this, "Collision", 0,
    //                   QVariantList() << m_parentNode->ID());
    //     qDebug() << "120912" << "collision on node" << m_parentNode->ID();
    // }

    else if ((m_state == rtxState_RXON) && (abs(RSSI - m_currentRX_RSSI) < 0.01)) {
        m_event->post(this, "message_dropped", 0,
                      QVariantList() << m_parentNode->ID() << message);
        m_state = rtxState_Free;
        m_currentRX_RSSI = 0;
        m_event->post(this, "Collision", 0,
                      QVariantList() << m_parentNode->ID());
        qDebug() << "120912" << "collision on node" << m_parentNode->ID();
    }

    else if ((m_state == rtxState_RXON) && (RSSI > m_currentRX_RSSI)) {
        m_event->post(this, "message_dropped", 0,
                      QVariantList() << m_parentNode->ID() << message);
        m_state = rtxState_Free;
        m_event->post(this, "SFD_RX_Up", 0,
                      QVariantList() << m_parentNode->ID() << message << RSSI);
        qDebug() << "120912" << "node" << m_parentNode->ID() << "start to receiving new message";
    }

    else {
        m_event->post(this, "message_dropped", 0,
                      QVariantList() << m_parentNode->ID() << message);
    }

}

void RTX::SFD_RX_Up_Event(byteArray message, double RSSI)
{
    if (m_state == rtxState_Free) {

        m_state = rtxState_RXON;
        m_currentRX_RSSI = RSSI;

        qDebug() << "SFD_RX_UP message" << message;

        m_event->post(this, "SFD_RX_Down", message.length() * 32,
                      QVariantList() << m_parentNode->ID() << message);
    }

}

void RTX::SFD_RX_Down_Event(byteArray message)
{
    if (m_state == rtxState_RXON) {
        m_state = rtxState_Free;
        // QByteArray new_message = message.mid(6);
        // FIXME: ugly code
        // TODO: IEvent->post can return some value which can help to delete event from queue
        m_event->post(this, "MessageReceived", 0,
                      // QVariantList() << m_parentNode->ID() << new_message);
                      QVariantList() << m_parentNode->ID() << message);
        qDebug() << "120912" << "node" << m_parentNode->ID() << "receive new message" << message;
    }
}

void RTX::SFD_TX_Up_Event(byteArray message, double TXPower)
{
    m_state = rtxState_TXON;
    qDebug() << "SFD TX UP message" << message;
}

void RTX::SFD_TX_Down_Event()
{
    m_state = rtxState_Free;
    m_event->post(this, "MessageSent", 0,
                  QVariantList() << m_parentNode->ID());
}

quint64 RTX::getLongAddr()
{
    return m_longAddr;
}

rtxState RTX::state()
{
    return m_state;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(rtx, RTX);
QT_END_NAMESPACE
