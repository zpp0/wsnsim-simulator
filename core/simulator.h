/**
 *
 * File: simulator.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QString>

#include "types.h"

class simulator
{

public:
    simulator(QString projectFileName);
    // ~simulator();
    
    void eval();

    // максимальное время работы симулятора
    VirtualTime getMaxSimulatorWorkTime();

private:
    void loadProject(QString file);

    // максимально-возможное время работы симулятора
    static VirtualTime m_maxSimulatorWorkTime;

};

#endif // SIMULATOR_H
