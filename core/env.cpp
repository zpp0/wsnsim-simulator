/**
 *
 * File: env.cpp
 * Description: Simulator Environment class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "env.h"

#include "simulator.h"

VirtualTime Env::time = 0;

VirtualTime Env::globalTime()
{
    return time;
}
