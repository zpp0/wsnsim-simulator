/**
 *
 * radioChannel.cpp
 *
 **/

#include "radioChannel.h"

#include "Irtx.h"

#define LAMBDA 0.122

bool moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    m_scene = (IScene*)isimulator->getEnvInterface(this, "IScene");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");
    m_simulator = isimulator;

    nodesHearTest();

    return true;
}

void radioChannel::send(INode* sender, byteArray message)
{
    qDebug() << "in radiochannel send";

    foreach (INode* listener, m_nodesLinks[sender]) {

        qDebug() << "add to local node channel" << listener->ID();

        m_nodesLocalChannel[listener] += message;

        double rssi_value = rssi(sender, listener);

        m_event->post(this, "newMessage", 0
                      QVariantList() << listener->ID() << message << rssi_value;
    }
}

double radioChannel::aroundPower(INode* listener)
{
    // TODO: написать
    // получить локальный канал узла listener
    // взять все передаваемые сообщения внутри канала
    // получить ID передающих узлов
    // посчитать максимальное RSSI от этих узлов
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

void radioChannel::nodesHearTest()
{
    //составляем списки слышимых узлов для каждого из узлов
    // WARNING: нужен список узлов
    // foreach (Node* node,
    QVector<INode*> nodes = m_scene->nodes();
    for (int i = 0; i < nodes.size(); i++) {
        //для данного узла проверим видит ли каждого из остальных
        for (int j = 0; j < nodes.size(); j++) {

            //с самим собой не сравниваем
            if (i == j)
                continue;

            double Rssi = rssi(nodes[i], nodes[j]);

            // проверяем слышат ли друг друга узлы с адресами mac1 и mac2 на расстоянии d
            if ((hear(Rssi, nodes[j]) == true)
                && m_nodesLinks[nodes[i]].indexOf(nodes[j]) == -1) {

                changeLink(true, nodes[i], nodes[j], Rssi);
            }

            // если не слышат, но раньше слышали
            else if ((hear(Rssi, nodes[j]) == false)
                     && m_nodesLinks[nodes[i]].indexOf(nodes[j]) != -1)
                changeLink(false, nodes[i], nodes[j], Rssi);
        }
    }
}

void radioChannel::changeLink(bool add, INode* node1, INode* node2, double rssi)
{
    // qDebug() << "in radiochannel changelink";

    if (add)
        m_nodesLinks[node1] += node2;
    else
        m_nodesLinks[node1] -= node2;

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
