/**
 *
 * File: node.h
 * Description: Simulator Node class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef NODE_H
#define NODE_H

#include "types.h"

class Node
{
public:
    Node(NodeID Id) : m_ID(Id) {}

    NodeID ID();

private:

    // служебный ID узла
    NodeID m_ID;
};

class NodesFactory
{
    Node* create();
private:
    static NodeID m_nextNodeID;
};

#endif // NODE_H
