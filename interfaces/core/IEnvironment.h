/**
 *
 * File: IEnvironment.h
 * Description: Environment interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IENVIRONMENT_H
#define IENVIRONMENT_H

#include "IModule.h"

struct IEnvironment : public IModule
{
	virtual ~IEnvironment() {}
};

#endif // IENVIRONMENT_H
