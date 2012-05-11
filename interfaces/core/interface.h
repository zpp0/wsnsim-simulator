/**
 *
 * File: interface.h
 * Description: Simulator interface bace class
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "types.h"

struct Interface
{
    Interface() { interfaceInfo.type = InterfaceType_Undefined; }
    virtual ~Interface() {};
    
    InterfaceInfo interfaceInfo;
};

#endif // INTERFACE_H
