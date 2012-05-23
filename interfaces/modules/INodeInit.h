/**
 *
 * File: INodePhy.h
 * Description: low level node's software interface
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef INODEPHY_H
#define INODEPHY_H

#include <QtCore>

#include "ISoftware.h"

class INodePHY : public ISoftware
{
public:
    ~INodePHY() {}
    INodePHY()
    {
        interfaceInfo.name = "INodePHY";
    }

    virtual void init() = 0;
};

#endif // INODEPHY_H
