/**
 *
 * IModule.h - Module interface
 *
 **/

#ifndef IMODULE_H
#define IMODULE_H

#include "interface.h"

#include "types.h"

struct IModule : public Interface
{
	virtual ~IModule() {}

    virtual ModuleInfo moduleInfo() const = 0;

	virtual bool moduleInit(QMap<QString, Interface*> importInterfaces,
                            QMap<QString, QString> params) = 0;
};

#endif // IMODULE_H
