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

    nodesHearTest();

    return true;
}

void radioChannel::send(Node* sender, byteArray message)
{
    qDebug() << "in radiochannel send";

    foreach (Node* listener, m_nodesLinks[sender]) {
        
        qDebug() << "add to local node channel" << listener->ID;

        m_nodesLocalChannel[listener] += message;

        double rssi_value = rssi(sender, listener);

        m_event->post(this, "newMessage",
                      QVariantList() << listener->ID() << message << rssi_value;
    }
}

byteArray radioChannel::listen(Node* listener)
{
    // FIXME: сделать нормально
    byteArray array;
    QVector<QByteArray> messages = m_nodesLocalChannel[listener];
    // FIXME: ARRRRRRGGGHHH
    if (messages.size() == 0)
        array += "1";

    if (messages.size() == 1)
        array = messages[0];
    return array;
}

double radioChannel::aroundPower(Node* listener)
{
    // TODO: написать
    // получить локальный канал узла listener
    // взять все передаваемые сообщения внутри канала
    // получить ID передающих узлов
    // посчитать максимальное RSSI от этих узлов
}

double radioChannel::rssi(Node* sender, Node* listener)
{
    double dist = m_scene->distance(sender, listener);

    // общее выражение Prx = Ptx*GTx*Grx(lambda/(4*pi*d))^2
    Irtx* rtx = (Irtx*)sender->getInterface((IModule*) this, "Irtx");
    // qDebug() << "get rtx interface from node" << rtx;
    double rssi = rtx->TXPower() + 10 * log10(pow(LAMBDA/(4*M_PI*dist), 2));

    // std::cerr <<  " sender "  << env::getNodeID(sender)
    // <<  " listener "  << env::getNodeID(listener)
              // <<  " rssi "  << rssi
              // <<  " distance "  << dist << std::endl;

    return rssi;

}

void radioChannel::nodesHearTest()
{
    //составляем списки слышимых узлов для каждого из узлов
    // WARNING: нужен список узлов
    // foreach (Node* node, 
    QVector<Node*> nodes = m_scene->nodes();
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

void radioChannel::changeLink(bool add, Node* node1, Node* node2, double rssi)
{
    // qDebug() << "in radiochannel changelink";

    if (add) {
        // qDebug() << "node" << node1->ID 
                 // << "listen node" << node2->ID;
        qDebug() << "node1" << node1->ID
                 << "hear node2" << node2->ID
                 << "with RSSI" << rssi;

        m_nodesLinks[node1] += node2;
        ChangeLink* event = new ChangeLink();
        qDebug() << "event count" << event::count;
        event->time = Env::time;
        event->eventNode = node1->ID;
        event->node2 = node2->ID;
        // event->recordable = true;

        // Env::queue.insert(event);

        qDebug() << "change link event number" << ChangeLink::count;
        log::writeLog(event);
        delete event;
    }
}

bool radioChannel::hear(double rssi, Node* listener)
{
    // qDebug() << "in radiochannel hear";
    Irtx* rtx = (Irtx*)listener->getInterface(NULL, "Irtx");
    // qDebug() << "get node rtx";
    if (rssi > rtx->RXSensivity()) {
        // qDebug() << "hear with RSSI" << rssi;
        return true;
    }
    else {
        // qDebug() << "not hear";
        return false;
    }
}



