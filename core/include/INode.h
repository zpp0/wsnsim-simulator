/**
 *
 * File: INode.h
 * Description: Simulator Node interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef INODE_H
#define INODE_H

#include "ICore.h"

#include "IModule.h"

struct INode : public ICore
{
    INode() { interfaceInfo.name = "INode"; }

    virtual NodeID ID() = 0;
    virtual IModule* getInterface(IModule* receiver, QString interfaceName) = 0;
};
// Q_DECLARE_INT

#endif // INODE_H
