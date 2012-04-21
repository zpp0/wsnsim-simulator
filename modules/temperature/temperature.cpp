/**
 *
 * temperature.cpp
 *
 **/

#include "temperature.h"

QString Temperature::moduleName() const
{
    return "Temperature";
}

QString Temperature::moduleVersion() const
{
    return "0.1";
}

QString Temperature::moduleDescription() const
{
    return "Модуль температуры";
}

bool Temperature::moduleInit(QList<ModuleParam> params)
{
    return true;
}

QList<QString> Temperature::moduleExportInterfaces() const
{
    QList<QString> tmp;
    return tmp;
}

QList<QString> Temperature::moduleImportInterfaces() const
{
    QList<QString> interfaces;
    return interfaces;
}

double Temperature::measure(double* coord, VirtualTime time)
{
    return 27;
}
