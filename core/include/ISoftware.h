/**
 *
 * ISoftware.h - node's Software interface
 *
 **/

#ifndef ISOFTWARE_H
#define ISOFTWARE_H

#include "IModule.h"

struct ISoftware : public IModule
{
	virtual ~ISoftware() {}
};

#endif // ISOFTWARE_H
