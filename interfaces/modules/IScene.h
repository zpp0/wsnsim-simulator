/**
 *
 * IScenel.h - Environment scene interface
 *
 **/

#ifndef ISCENE_H
#define ISCENE_H

#include <QtCore>

#include "IEnvironment.h"

#include "INode.h"
#include "INodesFactory.h"

class IScene : public IEnvironment, public QObject
{
    Q_OBJECT
public:
    IScene()
    {
        interfaceInfo.name = "IScene";
        interfaceInfo.events["nodePowerUp"]["NodeID"] = "NodeID";
        interfaceInfo.events["nodePowerUp"]["coordx"] = "double";
        interfaceInfo.events["nodePowerUp"]["coordy"] = "double";
    }

    virtual int dimension() = 0;
    virtual double* coord(INode* node) = 0;
    virtual double* size() = 0;
    virtual double distance(INode* node1, INode* node2) = 0;

    virtual quint16 nodesCount() = 0;
};
Q_DECLARE_INTERFACE(IScene,
                    "simulator.IScene/0.1");

#endif // ISCENE_H
