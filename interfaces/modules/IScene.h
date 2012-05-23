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

class IScene : public IEnvironment
{
public:
    virtual ~IScene() {}
    IScene()
    {
        interfaceInfo.name = "IScene";
        interfaceInfo.events["nodePowerUp"]["NodeID"] = "uint16";
        interfaceInfo.events["nodePowerUp"]["coordx"] = "double";
        interfaceInfo.events["nodePowerUp"]["coordy"] = "double";
    }

    virtual int dimension() = 0;
    virtual INode* node(NodeID nodeID) = 0;
    virtual double* coord(INode* node) = 0;
    virtual double* size() = 0;
    virtual double distance(INode* node1, INode* node2) = 0;
};

#endif // ISCENE_H
