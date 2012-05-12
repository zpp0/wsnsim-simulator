/**
 *
 * File: IModule.h
 * Description: Module interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IMODULE_H
#define IMODULE_H

#include "interface.h"

#include "ISimulator.h"

struct IModule : public Interface
{
	virtual ~IModule() {}

    ModuleInfo moduleInfo;
    
	virtual bool moduleInit(ISimulator* isimulator,
                            QMap<QString, QString> params) = 0;

    virtual void eventHandler(QString eventName, QVariantList params) {}
};

#endif // IMODULE_H
