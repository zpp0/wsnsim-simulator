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

        moduleInfo.params["temperatureMu"] = "double";
        moduleInfo.params["temperatureSigma"] = "double";

        moduleInfo.paramDescription["temperatureMu"] = "Параметр Mu нормального распределения в градусах (int32)";
        moduleInfo.paramDescription["temperatureSigma"] = "Параметр Sigma нормального распределения в градусах (int32)";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);
    
    /* virtual */ double measure(double* coord, VirtualTime time);

    /* virtual */ void eventHandler(QString name, QVariantList params) {}
    
private:
    double m_mu;
    double m_sigma;
};

#endif // TEMPERATURE_H
