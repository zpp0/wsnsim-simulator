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
    
    return true;
}


double Sensor::measure()
{
    double* coords = m_scene->coord(m_parentNode);
    return m_field->measure(coords, m_env->globalTime());
}
