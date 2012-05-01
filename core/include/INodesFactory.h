/**
 *
 * File: INodesFactory.h
 * Description: Simulator Node interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef INODESFACTORY_H
#define INODESFACTORY_H

#include "ICore.h"

class INode;

struct INodesFactory : public ICore
{
    INodesFactory() { interfaceInfo.name = "INodesFactory"; }

    virtual INode* create() = 0;
};
// Q_DECLARE_INT

#endif // INODESFACTORY_H
