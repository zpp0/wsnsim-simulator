/**
 *
 * File: env.h
 * Description: Simulator Environment class
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ENV_H
#define ENV_H

#include <QList>

#include "IEnv.h"

// Класс среды

// FIXME: is it hack?
struct Env : public IEnv
{
    /* virtual */ VirtualTime globalTime();
    
    // реальное время в системе
    static VirtualTime time;
};

#endif // ENV_H
