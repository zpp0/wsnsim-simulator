/**
 *
 * rtx.cpp
 *
 **/

#include "rtx.h"

#include "env.h"
#include "IScene.h"

#include "log.h"


quint64 Irtx::SFD_RX_Up::count = 0;
quint64 Irtx::SFD_RX_Down::count = 0;
quint64 Irtx::SFD_TX_Up::count = 0;
quint64 Irtx::SFD_TX_Down::count = 0;
quint64 Irtx::Collision::count = 0;
quint64 Irtx::CCATest::count = 0;


QString RTX::moduleName() const
{
    return "RTX";
}

QString RTX::moduleVersion() const
{
    return "0.1";
}

QString RTX::moduleDescription() const
{
    return "Модуль радио приемника-передатчика стандарта 802.15.4";
}

bool RTX::moduleInit(QList<ModuleParam> params)
{
    m_channel = (IRadioChannel*)Env::getInterface((IModule*)this, "IRadioChannel");

    m_state = rtxState_Free;

    int RXSensivity = -90;
    int TXPower = 0;
    int CCAThreshold = -45;

    foreach (ModuleParam param, params) {
        if (param.name == "RXSensivity")
            RXSensivity = param.value.toInt();
        if (param.name == "TXPower")
            TXPower = param.value.toInt();
        if (param.name == "CCAThreshold")
            CCAThreshold = param.value.toInt();
    }

    qDebug("RTX params gotten:");
    qDebug("RXSensivity: %i TXPower: %i CCAThreshold: %i",
           RXSensivity, TXPower, CCAThreshold);

    m_RXSensivity = RXSensivity;
    m_TXPower = TXPower;
    m_CCAThreshold = CCAThreshold;
    // m_channel = 11;

    return true;
}

QList<QString> RTX::moduleExportInterfaces() const
{
    QList<QString> tmp;
    return tmp;
}

QList<QString> RTX::moduleImportInterfaces() const
{
    QList<QString> interfaces;
    interfaces += "IRadioChannel";
    return interfaces;
}

QString RTX::deviceName() const
{
    return "cc2420";
}

QList<InterruptHandler> RTX::interrupts()
{
    QList<InterruptHandler> tmp;
    return tmp;
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

bool RTX::clearChannel()
{
    byteArray message = m_channel->listen(m_parentNode);
    // qDebug() << "get message" << message << "size" << message.size();
    return message.size() > 0;
}

void RTX::SFD_TX_Down::process()
{

    // TODO: ЭТО РАБОТАЕТ ТОЛЬКО ПОТОМУ ЧТО НЕ РЕАЛИЗОВАНА ПРОВЕРКА ИМПОРТИРУЕМЫХ МОДУЛЕЙ getInterface
    IScene* scene = (IScene*)Env::getInterface(NULL, "IScene");

    QVector<Node*> nodes = scene->nodes();

    Node* rtxNode = NULL;

    foreach (Node* node, nodes)
        if (node->ID == eventNode)
            rtxNode = node;

    qDebug() << "in TX Down interrupt node" << rtxNode->ID;

    RTX* rtx = (RTX*)rtxNode->getInterface(NULL, "Irtx");

    rtx->m_state = RTX::rtxState_Free;
    
    // Node* node = eventNode 

    qDebug() << "rtx state set Free";

    log::writeLog(this);
}

void RTX::SFD_RX_Up::process()
{
    // TODO: ЭТО РАБОТАЕТ ТОЛЬКО ПОТОМУ ЧТО НЕ РЕАЛИЗОВАНА ПРОВЕРКА ИМПОРТИРУЕМЫХ МОДУЛЕЙ getInterface
    IScene* scene = (IScene*)Env::getInterface(NULL, "IScene");

    QVector<Node*> nodes = scene->nodes();

    Node* rtxNode = NULL;

    foreach (Node* node, nodes)
        if (node->ID == eventNode)
            rtxNode = node;

    RTX* rtx = (RTX*)rtxNode->getInterface(NULL, "Irtx");

    if ((rtx->m_state == RTX::rtxState_Free)
        || (rtx->m_state == RTX::rtxState_RXON))
    {
        qDebug() << "in RX UP interrupt node" << rtxNode->ID;

        rtx->m_state = RTX::rtxState_RXON;
    
        // Node* node = eventNode 

        qDebug() << "rtx state set RXON";
        log::writeLog(this);
    }
}

void RTX::SFD_RX_Down::process()
{
    // TODO: ЭТО РАБОТАЕТ ТОЛЬКО ПОТОМУ ЧТО НЕ РЕАЛИЗОВАНА ПРОВЕРКА ИМПОРТИРУЕМЫХ МОДУЛЕЙ getInterface
    IScene* scene = (IScene*)Env::getInterface(NULL, "IScene");

    QVector<Node*> nodes = scene->nodes();

    Node* rtxNode = NULL;

    foreach (Node* node, nodes)
        if (node->ID == eventNode)
            rtxNode = node;

    RTX* rtx = (RTX*)rtxNode->getInterface(NULL, "Irtx");

    log::writeLog(this);

    if (rtx->m_state == RTX::rtxState_RXON) {
        // TODO: проверить что не возникла коллизия
        if (rtx->clearChannel()) {
            qDebug() << "in RX Down interrupt node" << rtxNode->ID;

            rtx->m_state = RTX::rtxState_Free;
    
            qDebug() << "rtx state set Free";

        }
        else {
            qDebug() << "collision on node" << rtxNode->ID;

            Collision* event = new Collision();
            qDebug() << "event count" << event::count;
            event->time = Env::time;
            event->eventNode = eventNode;
            // event->recordable = true;

            log::writeLog(event);

            delete event;
            // Env::queue.insert(event);

            rtx->m_state = RTX::rtxState_Free;

            qDebug() << "rtx state set Free";
        }
    }
}
