/**
 *
 * radioChannel.cpp
 *
 **/

#include "radioChannel.h"

#include "Irtx.h"

#define LAMBDA 0.122

bool radioChannel::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    m_scene = (IScene*)isimulator->getEnvInterface(this, "IScene");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");
    m_simulator = isimulator;

    return true;
}

void radioChannel::send(INode* sender, byteArray message)
{
    qDebug() << "in radiochannel send";

    foreach (INode* listener, m_nodesLinks[sender]) {

        qDebug() << "add to local node channel" << listener->ID();

        m_nodesLocalChannel[listener] += message;

        double rssi_value = rssi(sender, listener);

        m_event->post(this, "newMessage", 0,
                      QVariantList() << listener->ID() << message << rssi_value);
    }
}

double radioChannel::aroundPower(INode* listener)
{
    // TODO: написать
    // получить локальный канал узла listener
    // взять все передаваемые сообщения внутри канала
    // получить ID передающих узлов
    // посчитать максимальное RSSI от этих узлов
    // NOTE: test
    return 55;
}

double radioChannel::rssi(INode* sender, INode* listener)
{
    double dist = m_scene->distance(sender, listener);

    // общее выражение Prx = Ptx*GTx*Grx(lambda/(4*pi*d))^2
    Irtx* rtx = (Irtx*)m_simulator->getNodeInterface(this, sender, "Irtx");
    // qDebug() << "get rtx interface from node" << rtx;
    double rssi = rtx->TXPower() + 10 * log10(pow(LAMBDA/(4*M_PI*dist), 2));

    return rssi;
}

void radioChannel::nodesHearingUpdate(INode* node)
{
    // для данного узла проверим видит ли каждого из остальных
    foreach (INode* listener, m_radioNodes) {

        double rssi_value = rssi(node, listener);

        // проверяем слышат ли друг друга узлы с адресами mac1 и mac2 на расстоянии d
        if ((hear(rssi_value, listener) == true)
            && m_nodesLinks[node].indexOf(listener) == -1)
            changeLink(true, node, listener, rssi_value);

        // если не слышат, но раньше слышали
        else if ((hear(rssi_value, listener) == false)
                 && m_nodesLinks[node].indexOf(listener) != -1)
            changeLink(false, node, listener, rssi_value);
    }
}

void radioChannel::changeLink(bool add, INode* node1, INode* node2, double rssi)
{
    // qDebug() << "in radiochannel changelink";

    if (add)
        m_nodesLinks[node1] += node2;
    else
        m_nodesLinks[node1].remove(m_nodesLinks[node1].indexOf(node2));

    qDebug() << "node1" << node1->ID()
             << "hear node2" << node2->ID()
             << "with RSSI" << rssi;

    m_event->post(this, "ChangeLink", 0,
                  QVariantList() << node1->ID() << node2->ID());
}

bool radioChannel::hear(double rssi, INode* listener)
{
    // qDebug() << "in radiochannel hear";
    Irtx* rtx = (Irtx*)m_simulator->getNodeInterface(this, listener, "Irtx");

    return rssi > rtx->RXSensivity();
}

void radioChannel::eventHandler(QString eventName, QVariantList params)
{
    if (eventName == "nodePowerUp")
        // FIXME: very ugly
        nodePowerUp_Event(params[0].toUInt(), params[1].toDouble(), params[2].toDouble());
}

void radioChannel::nodePowerUp_Event(NodeID nodeID, double coordx, double coordy)
{
    INode* node = m_scene->node(nodeID);
    nodesHearingUpdate(node);
    m_radioNodes += node;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(radioChannel, radioChannel);
QT_END_NAMESPACE
