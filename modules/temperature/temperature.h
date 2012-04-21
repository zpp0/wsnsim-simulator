/**
 *
 * temperature.h
 *
 **/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QtCore>

#include "IField.h"

class Temperature : public IField
{
public:
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QString deviceName() const;
    virtual QList<InterruptHandler> interrupts();

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    virtual double measure(double* coord, VirtualTime time) = 0;
};
// Q_EXPORT_PLUGIN(Temperature);

#endif // TEMPERATURE_H
