/**
 *
 * File: ISensor.h
 * Description: Sensor interface
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef ISENSOR_H
#define ISENSOR_H

#include <QtCore>

#include "IHardware.h"

class IAsyncSensor : public IHardware
{
public:
    ~IAsyncSensor() {}
    IAsyncSensor()
    {
        interfaceInfo.name = "IAsyncSensor";

        interfaceInfo.events["measuring_start"]
            << qMakePair(QString("NodeID"), QString("uint16"));

        interfaceInfo.events["measuring_get_result"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("sensorName"), QString("string"))
            << qMakePair(QString("measureResult"), QString("double"));

    }

    virtual void measure() = 0;
};

#endif // IRTX_H