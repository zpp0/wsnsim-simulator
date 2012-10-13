/**
 *
 * File: env.h
 * Description: Simulator Environment class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef ENV_H
#define ENV_H

#include <QList>
#include "types.h"

// Класс среды
struct Env
{
    VirtualTime globalTime();
    // реальное время в системе
    static VirtualTime time;
};

#endif // ENV_H
