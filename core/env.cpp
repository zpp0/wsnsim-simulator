/**
 *
 * File: env.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "env.h"

#include "simulator.h"

VirtualTime Env::time = 0;

VirtualTime Env::globalTime()
{
    return time;
}
