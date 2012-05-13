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

class radioChannel : public IRadioChannel
{
    Q_OBJECT
    Q_INTERFACES(IRadioChannel)

public:
    radioChannel()
    {
        moduleInfo.name = "Radio Channel";
        moduleInfo.version = "0.1";
        moduleInfo.description = "";
        moduleInfo.exportInterface = "IRadioChannel";
        moduleInfo.importInterfaces += "IScene";
        moduleInfo.importInterfaces += "Irtx";
        moduleInfo.importInterfaces += "IEvent";
        moduleInfo.handledEvents += "nodePowerUp";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void send(INode* sender, byteArray message);
    /* virtual */ byteArray listen(INode* listener);
    /* virtual */ double aroundPower(INode* listener);

    /* virtual */ void eventHandler(QString eventName, QVariantList params);

private:
    double rssi(INode* sender, INode* listener);
    void nodesHearingUpdate(INode* node);
    void changeLink(bool add, INode* node1, INode* node2, double rssi);
    bool hear(double rssi, INode* listener);

    void nodePowerUp_Event(INode* node, double coordx, double coordy);

    IScene* m_scene;
    IEvent* m_event;

    ISimulator* m_simulator;

    QVector<INode*> m_radioNodes;
    QHash<INode*, QVector<INode*> > m_nodesLinks;
    QHash<INode*, QVector<QByteArray> > m_nodesLocalChannel;
};

#endif // RADIOCHANNEL_H
