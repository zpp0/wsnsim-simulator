/**
 *
 * ISensor.h
 *
 **/

#ifndef ISENSOR_H
#define ISENSOR_H

#include <QtCore>

#include "IHardware.h"

class ISensor : public IHardware
{
public:

    virtual double measure() = 0;
};

Q_DECLARE_INTERFACE(ISensor,
                    "simulator.ISensor/0.1");

#endif // IRTX_H
