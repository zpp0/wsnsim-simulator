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

#include "IModule.h"

// Класс среды

// FIXME: is it hack?
struct Env : public IEnv
{
    /* virtual */ VirtualTime time();
    /* virtual */ IModule* getInterface(IModule* receiver, QString interfaceName);
    
    // реальное время в системе
    static VirtualTime time;
};

#endif // ENV_H
