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

struct IModule : public Interface
{
	virtual ~IModule() {}

    ModuleInfo moduleInfo;
    
	virtual bool moduleInit(QMap<QString, Interface*> importInterfaces,
                            QMap<QString, QString> params) = 0;
};

#endif // IMODULE_H
