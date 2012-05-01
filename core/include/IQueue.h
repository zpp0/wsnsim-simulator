/**
 *
 * File: IQueue.h
 * Description: Simulator Event queue interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IQUEUE_H
#define IQUEUE_H

#include "ICore.h"

#include "event.h"

class IQueue : public ICore
{
public:
	virtual ~IQueue() {}

    virtual QString interfaceName() const { return "IQueue"; }

    virtual void insert(event* e) = 0;
};
// Q_DECLARE_INT

#endif // IQUEUE_H
