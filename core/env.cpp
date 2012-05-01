/**
 *
 * File: env.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "env.h"

VirtualTime Env::time;

VirtualTime Env::time()
{
    return time;
}

IModule* Env::getInterface(IModule* receiver, QString interfaceName)
{
    // TODO: do it
    return NULL;
}
