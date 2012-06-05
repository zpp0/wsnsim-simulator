/**
 *
 * sensor.cpp
 *
 **/

#include "sensor.h"

bool Sensor::moduleInit(ISimulator* isimulator,
                QMap<QString, QString> params)
{
    m_field = (IField*)isimulator->getEnvInterface(this, "IField");
    m_scene = (IScene*)isimulator->getEnvInterface(this, "IScene");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");
    m_env = (IEnv*)isimulator->getCoreInterface(this, "IEnv");
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");

    m_mu = params["measuringTimeMu"].toULong();
    m_sigma = params["measuringTimeSigma"].toULong();

    m_sensorName = "temperatureSensor";

    srand((long)this);

    return true;
}

void Sensor::measure()
{
    double x = rand();
    double y = rand();

    double z = cos(2 * M_PI * x) * sqrt(2 * log(y));

    qDebug() << "delay" << x << y << z;

    VirtualTime delay = m_mu + m_sigma * z;

    m_event->post(this, "measuring_start", delay,
                  QVariantList() << m_parentNode->ID());
}

void Sensor::eventHandler(QString name, QVariantList params)
{
    if (name == "measuring_start") {
        double value = m_field->measure(m_scene->coord(m_parentNode),
                                        m_env->globalTime());

        qDebug() << "asd";
        m_event->post(this, "measuring_get_result", 0,
                      QVariantList() << m_parentNode->ID() << m_sensorName << value);
    }
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(sensor, Sensor);
QT_END_NAMESPACE
