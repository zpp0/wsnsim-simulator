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

#include "types.h"

class INode : public ICore
{
public:
	virtual ~INode() {}

    virtual QString interfaceName() const { return "INode"; }

    virtual NodeID ID() = 0;
    virtual IModule* getInterface(QString requestingModuleName, QString requestedInterfaceName) = 0;
};
// Q_DECLARE_INT

#endif // INODE_H
