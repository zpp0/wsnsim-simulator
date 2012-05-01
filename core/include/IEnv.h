/**
 *
 * File: IEnv.h
 * Description: Simulator Environment interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IENV_H
#define IENV_H

#include "ICore.h"

#include "IModule.h"

#include "types.h"

class IEnv : public ICore
{
public:
	virtual ~IEnv() {}

    virtual QString interfaceName() const { return "IEnv"; }

    virtual VirtualTime time() = 0;
    virtual IModule* getInterface(QString requestingModuleName, QString requestedInterfaceName) = 0;
};
// Q_DECLARE_INT

#endif // IENV_H
