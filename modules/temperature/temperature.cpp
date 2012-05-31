/**
 *
 * temperature.cpp
 *
 **/

#include "temperature.h"

bool Temperature::moduleInit(ISimulator* isimulator,
                             QMap<QString, QString> params)
{
    m_mu = params["temperatureMu"].toInt();
    m_sigma = params["measuringTimeSigma"].toInt();

    srand((long)this);
    
    return true;
}

double Temperature::measure(double* coord, VirtualTime time)
{
    double x = rand();
    double y = rand();

    double z = cos(2 * M_PI * x) * sqrt(-2 * log(y));
        
    return m_mu + m_sigma * z;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(temperature, Temperature);
QT_END_NAMESPACE
