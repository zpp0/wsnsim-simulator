/**
 *
 * File: IEnv.h
 * Description: Simulator Environment interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IENV_H
#define IENV_H

#include "ICore.h"

class IModule;

struct IEnv : public ICore
{
    IEnv() { interfaceInfo.name = "IEnv"; }
    
    virtual VirtualTime globalTime() = 0;
};
// Q_DECLARE_INT

#endif // IENV_H
