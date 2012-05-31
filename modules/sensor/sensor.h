/**
 *
 * File: sensor.h
 * Description: Sensor module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore>

#include "ISensor.h"
#include "IField.h"

#include "IScene.h"
#include "INode.h"
#include "IEvent.h"
#include "IEnv.h"

class Sensor : public QObject, public ISensor
{
    Q_OBJECT
	Q_INTERFACES(IModule)

public:
    Sensor()
    {
        moduleInfo.name = "Sensor";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Точный датчик измеряемой величины с не бесконечной скоростью измерения";
        moduleInfo.exportInterface = "ISensor";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IScene";
        moduleInfo.importInterfaces += "IField";
        moduleInfo.importInterfaces += "IEvent";
        moduleInfo.importInterfaces += "IEnv";

        moduleInfo.params["measuringTimeMu"] = "uint64";
        moduleInfo.params["measuringTimeSigma"] = "uint64";

        moduleInfo.paramDescription["measuringTimeMu"] = "Параметр Mu нормального распределения в мкс (uint64)";
        moduleInfo.paramDescription["measuringTimeSigma"] = "Параметр Sigma нормального распределения (uint64)";

    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    virtual double measure();

private:
    IField* m_field;
    IScene* m_scene;
    IEvent* m_event;
    IEnv* m_env;
    INode* m_parentNode;

};

#endif // SENSOR_H
