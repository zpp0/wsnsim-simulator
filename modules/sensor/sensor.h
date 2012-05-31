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

#include "IAsyncSensor.h"

#include "IField.h"

#include "IScene.h"
#include "INode.h"
#include "IEvent.h"
#include "IEnv.h"

class Sensor : public QObject, public IAsyncSensor
{
    Q_OBJECT
	Q_INTERFACES(IModule)

public:
    Sensor()
    {
        moduleInfo.name = "Sensor";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Точный датчик измеряемой величины с не бесконечной скоростью измерения";

        moduleInfo.exportInterface = "IAsyncSensor";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IScene";
        moduleInfo.importInterfaces += "IField";
        moduleInfo.importInterfaces += "IEvent";
        moduleInfo.importInterfaces += "IEnv";

        moduleInfo.params["measuringTimeMu"] = "uint64";
        moduleInfo.params["measuringTimeSigma"] = "uint64";

        moduleInfo.paramDescription["measuringTimeMu"] = "Параметр Mu нормального распределения в мкс (uint64)";
        moduleInfo.paramDescription["measuringTimeSigma"] = "Параметр Sigma нормального распределения (uint64)";

        moduleInfo.handledEvents += "measuring_start";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void eventHandler(QString name, QVariantList params);
    
    /* virtual */ void measure();

private:
    IField* m_field;
    IScene* m_scene;
    IEvent* m_event;
    IEnv* m_env;
    INode* m_parentNode;

    QString m_sensorName;
    
    quint64 m_mu;
    quint64 m_sigma;
};

#endif // SENSOR_H
