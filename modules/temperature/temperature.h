/**
 *
 * temperature.h
 *
 **/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QtCore>

#include "IField.h"

class Temperature : public QObject, public IField
{
    Q_OBJECT
	Q_INTERFACES(IModule)

public:
    Temperature()
    {
        moduleInfo.name = "Temperature";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Температура среды";
        moduleInfo.exportInterface = "IField";

        moduleInfo.params["temperatureMu"] = "int32";
        moduleInfo.params["temperatureSigma"] = "int32";

        moduleInfo.paramDescription["temperatureMu"] = "Параметр Mu нормального распределения в градусах (int32)";
        moduleInfo.paramDescription["temperatureSigma"] = "Параметр Sigma нормального распределения в градусах (int32)";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);
    
    /* virtual */ double measure(double* coord, VirtualTime time);

    /* virtual */ void eventHandler(QString name, QVariantList params) {}
    
private:
    int m_mu;
    int m_sigma;
};

#endif // TEMPERATURE_H
