/**
 *
 * File: node.cpp
 * Description: Simulator Node class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "node.h"

#include "simulator.h"

NodeID Node::ID()
{
    return m_ID;
}

NodeID NodesFactory::m_nextNodeID = 0;

Node* NodesFactory::create()
{
    Node* node = new Node(m_nextNodeID);
    m_nextNodeID++;
    Simulator::registerNode(node);
    // TODO: is need to set up modules?
    return node;
}
