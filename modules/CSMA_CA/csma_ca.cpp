/**
 *
 * File: csma_ca.cpp
 * Description: CSMA CA module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "csma_ca.h"

bool CSMA_CA::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    m_rtx = (Irtx*)isimulator->getNodeInterface(this, m_parentNode, "Irtx");
    m_timer = (ITimer*)isimulator->getNodeInterface(this, m_parentNode, "ITimer");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");

    qsrand((long)this);

    return true;
}

void CSMA_CA::sendMessage(byteArray message)
{
    m_event->post(this, "CSMA_begin", 0,
                  QVariantList() << m_parentNode->ID());

    m_message = message;

    // сбрасываем переменные алгоритма CSMA-CA
    m_BE = macMinBE;
    m_NB = 0;

    VirtualTime wait = delay();

    m_event->post(this, "CSMA_delay", 0,
                  QVariantList() << m_parentNode->ID() << m_BE << m_NB << wait);

    m_timer->start(wait, "CSMA-CA");
}

VirtualTime CSMA_CA::delay()
{
    int periods = qrand() % (int)(pow(2, m_BE) - 1);
    VirtualTime wait = periods * aUnitBackoffPeriod * ByteSendingTime / 2;
    return wait;
}

void CSMA_CA::eventHandler(QString name, QVariantList params)
{
    // FIXME: ugly code
    if (name == "timerInterrupt")
        if (params[1].toString() == "CSMA-CA")
            timerInterrupt();
}

void CSMA_CA::timerInterrupt()
{
    if (m_rtx->CCA()) {
        m_event->post(this, "CSMA_success", 0,
                      QVariantList() << m_parentNode->ID());

        m_rtx->startTX(m_message);
    }

    else {
        m_NB++;
        m_BE = std::min(m_BE + 1, aMaxBE);

        VirtualTime wait = delay();

        if (m_NB < macMaxCSMABackoffs) {
            m_event->post(this, "CSMA_delay", 0,
                          QVariantList() << m_parentNode->ID() << m_BE << m_NB << wait);

            m_timer->start(wait, "CSMA-CA");
        }

        else
            m_event->post(this, "CSMA_fail", 0,
                          QVariantList() << m_parentNode->ID());
    }
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(csma_ca, CSMA_CA);
QT_END_NAMESPACE
