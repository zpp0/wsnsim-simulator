/**
 *
 * File: ICore.h
 * Description: Simulator Core Interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ICORE_H
#define ICORE_H

#include "interface.h"

struct ICore : public Interface
{
	virtual ~ICore() {}
};

#endif // ICORE_H
