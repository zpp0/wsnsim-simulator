/**
 *
 * IScenel.h - Environment scene interface
 *
 **/

#ifndef ISCENE_H
#define ISCENE_H

#include <QtCore>

#include "IEnvironment.h"

#include "nodeEvent.h"

#include "node.h"

class IScene : public IEnvironment
{
public:
    virtual int dimension() = 0;
    virtual double* coord(Node* node) = 0;
    virtual double* size() = 0;
    virtual double distance(Node* node1, Node* node2) = 0;

    virtual quint16 nodesCount() = 0;

    class nodePowerUp : public nodeEvent
    {
    public:
        nodePowerUp() { count++; }

        static quint64 count;


        double coords[2];
        virtual QString eventName() const { return "nodePowerUp"; }
        virtual void record(QDataStream& stream) { stream << eventNode << coords[0] << coords[1]; }
        virtual void process();
    };
    
    // TODO: THIS WILL BE DELETED!
    virtual QVector<Node*> nodes() = 0;
    
};
Q_DECLARE_INTERFACE(IScene,
                    "simulator.IScene/0.1");

#endif // ISCENE_H
