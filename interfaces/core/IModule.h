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

    virtual void eventHandler(QString eventName, QVariantList params) = 0;
};
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IModule,
                    "simulator.IModule/1.0");
QT_END_NAMESPACE

#endif // IMODULE_H
