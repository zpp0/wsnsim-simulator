/**
 *
 * radioChannel.h
 *
 **/

#ifndef RADIOCHANNEL_H
#define RADIOCHANNEL_H

#include <QtCore>

#include "IRadioChannel.h"

#include "IScene.h"
#include "IEvent.h"

class radioChannel : public QObject, public IRadioChannel
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    radioChannel()
    {
        moduleInfo.name = "Radio Channel";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Радиоканал в частотном диапазоне 2.4ГГц";
        moduleInfo.exportInterface = "IRadioChannel";
        moduleInfo.importInterfaces += "IScene";
        moduleInfo.importInterfaces += "Irtx";
        moduleInfo.importInterfaces += "IEvent";
        moduleInfo.handledEvents += "nodePowerUp";
        moduleInfo.handledEvents += "Collision";
        moduleInfo.handledEvents += "MessageReceived";
        moduleInfo.handledEvents += "message_dropped";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void send(INode* sender, byteArray message);
    /* virtual */ double aroundPower(INode* listener);
    /* virtual */ bool isNetworkConnected(QVector<INode*> nodes);

    /* virtual */ void eventHandler(QString eventName, QVariantList params);

private:
    double rssi(INode* sender, INode* listener);
    void nodesHearingUpdate(INode* node);
    void changeLink(bool add, INode* node1, INode* node2, double rssi);
    bool hear(double rssi, INode* listener);

    bool isPathExist(int i, int j, QVector<QVector<bool> > nodesLinks);

    void nodePowerUp_Event(NodeID nodeID);

    IScene* m_scene;
    IEvent* m_event;

    ISimulator* m_simulator;

    QVector<INode*> m_radioNodes;
    QHash<INode*, QVector<INode*> > m_nodesLinks;
    QHash<INode*, QVector<QByteArray> > m_nodesLocalChannel;
};

#endif // RADIOCHANNEL_H
