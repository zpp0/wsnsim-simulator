/**
 *
 * sensor.cpp
 *
 **/

#include "sensor.h"

#include "IField.h"

#include "env.h"
#include "IScene.h"


QString Sensor::moduleName() const
{
    return "Sensor";
}

QString Sensor::moduleVersion() const
{
    return "0.1";
}

QString Sensor::moduleDescription() const
{
    return "Модуль датчика без погрешностей";
}

bool Sensor::moduleInit(QList<ModuleParam> params)
{
    m_field = (IField*)Env::getInterface((IModule*)this, "IField");
    m_scene = (IScene*)Env::getInterface((IModule*)this, "IScene");

    return true;
}

QList<QString> Sensor::moduleExportInterfaces() const
{
    QList<QString> tmp;
    return tmp;
}

QList<QString> Sensor::moduleImportInterfaces() const
{
    QList<QString> interfaces;
    interfaces += "IField";
    interfaces += "IScene";
    return interfaces;
}

QString Sensor::deviceName() const
{
    return "sensor";
}

QList<InterruptHandler> Sensor::interrupts()
{
    QList<InterruptHandler> tmp;
    return tmp;
}

double Sensor::measure()
{
    double* coords = m_scene->getCoord(m_parentNode);
    return m_field->measure(coords, Env::time);
}
