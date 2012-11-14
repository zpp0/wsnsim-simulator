/**
 *
 * File: nodes.h
 * Description: nodes types declarations
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef NODES_H
#define NODES_H

#include <QList>

#include "module.h"

typedef quint16 NodeID;

typedef QList<ModuleID> NodePlatform;
typedef QList<ModuleID> NodeApplication;

struct NodeType
{
    NodePlatform platform;
    NodeApplication application;
};

#endif // NODES_H
