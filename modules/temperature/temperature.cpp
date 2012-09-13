/**
 *
 * temperature.cpp
 *
 **/

#include "temperature.h"

bool Temperature::moduleInit(ISimulator* isimulator,
                             QMap<QString, QString> params)
{
    m_mu = params["temperatureMu"].toDouble();
    m_sigma = params["temperatureSigma"].toDouble();

    return true;
}

double Temperature::measure(double* coord, VirtualTime time)
{
    srand((long)this + time + (long)coord);

    double x = ((double)rand()) / ((double) RAND_MAX);
    double y = ((double)rand()) / ((double) RAND_MAX);

    double z = cos(2 * M_PI * x) * sqrt(-2 * log(y));

    qDebug() << "random z" << z << m_mu + m_sigma * z;

    return m_mu + m_sigma * z;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(temperature, Temperature);
QT_END_NAMESPACE
