/**
 *
 * IEnvironment.h - Environment interface
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
