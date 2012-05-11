/**
 *
 * File: ISoftware.h
 * Description: node's Software interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
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
