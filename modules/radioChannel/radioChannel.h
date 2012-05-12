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

/**
 * WARNING:
 * в интерфейсе нет оповещения о включении/выключении узла
 * в интерфейсе нет оповещения о перемещении узла
 * в интерфейсе нет оповещения о смене исходящей мощности узла
 **/

class radioChannel : public IRadioChannel, public QObject
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
        QList<QString> dependence;
        dependence += "IScene";
        dependence += "Irtx";
        dependence += "IEvent";
        moduleInfo.importInterfaces = dependence;
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void send(INode* sender, byteArray message);
    /* virtual */ byteArray listen(INode* listener);
    /* virtual */ double aroundPower(INode* listener);

private:
    double rssi(INode* sender, INode* listener);
    void nodesHearTest();
    void changeLink(bool add, INode* node1, INode* node2, double rssi);
    bool hear(double rssi, INode* listener);

    IScene* m_scene;
    IEvent* m_event;

    ISimulator* m_simulator;

    QHash<INode*, QVector<INode*> > m_nodesLinks;
    QHash<INode*, QVector<QByteArray> > m_nodesLocalChannel;
};
// Q_EXPORT_PLUGIN(radioChannel)

#endif // RADIOCHANNEL_H
