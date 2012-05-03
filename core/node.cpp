/**
 *
 * File: node.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "node.h"

#include "simulator.h"

NodeID Node::ID()
{
    return m_ID;
}

IModule* Node::getInterface(IModule* receiver, QString interfaceName)
{
    return Simulator::getNodeInterface(receiver, this, interfaceName);
}

NodeID NodesFactory::m_nextNodeID = 0;

INode* NodesFactory::create()
{
    Node* node = new Node(m_nextNodeID);
    m_nextNodeID++;
    Simulator::registerNode(node);
    // TODO: is need to set up modules?
    return (INode*) node;
}
