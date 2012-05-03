/**
 *
 * File: node.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef NODE_H
#define NODE_H

#include "INode.h"

class Node : public INode
{
public:
    Node(NodeID Id) : m_ID(Id) {}

    /* virtual */ NodeID ID();
    /* virtual */ IModule* getInterface(IModule* receiver, QString interfaceName);

private:

    // служебный ID узла
    NodeID m_ID;
};

#include "INodesFactory.h"

class NodesFactory : public INodesFactory
{
    // ~NodeFactory();

    /* virtual */ INode* create();
private:
    static NodeID m_nextNodeID;
    // static QList<Node*> m_nodes;
};

#endif // NODE_H
