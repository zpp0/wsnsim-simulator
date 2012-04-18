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

/**
 * WARNING:
 * в интерфейсе нет оповещения о включении/выключении узла
 * в интерфейсе нет оповещения о перемещении узла
 * в интерфейсе нет оповещения о смене исходящей мощности узла
 * интерфейсам нужны строковые имена, чтобы можно было запросить их из объекта
 **/

class radioChannel : public IRadioChannel
{
    Q_OBJECT
    Q_INTERFACES(IRadioChannel)

public:
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    virtual void send(Node* sender, byteArray message);

    virtual byteArray listen(Node* listener);

    virtual double aroundPower(Node* listener);

private:
    double rssi(Node* sender, Node* listener);

    void nodesHearTest();

    void changeLink(bool add, Node* node1, Node* node2, double rssi);

    bool hear(double rssi, Node* listener);

    IScene* m_scene;
    
    QHash<Node*, QVector<Node*> > m_nodesLinks;

    QHash<Node*, QVector<QByteArray> > m_nodesLocalChannel;
};
// Q_EXPORT_PLUGIN(radioChannel)

#endif // RADIOCHANNEL_H
