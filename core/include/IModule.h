/**
 *
 * IModule.h - Module interface
 *
 **/

#ifndef IMODULE_H
#define IMODULE_H

#include <QtCore>

#include "projectParams.h"

class IModule : public QObject
{
    Q_OBJECT
public:
	virtual ~IModule() {}

	virtual QString moduleName() const = 0;

	virtual QString moduleVersion() const = 0;

    virtual QString moduleDescription() const = 0;

	virtual bool moduleInit(QList<ModuleParam> params) = 0;

    virtual QList<QString> moduleExportInterfaces() const = 0;

    virtual QList<QString> moduleImportInterfaces() const = 0;
};


#endif // IMODULE_H
