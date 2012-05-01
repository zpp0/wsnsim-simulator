/**
 *
 * File: IHardware.h
 * Description: node's Hardware interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IHARDWARE_H
#define IHARDWARE_H

#include "IModule.h"

struct IHardware : public IModule
{
	virtual ~IHardware() {}
};

#endif // IHARDWARE_H
