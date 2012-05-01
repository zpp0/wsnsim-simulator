/**
 *
 * File: ILog.h
 * Description: Simulator Log interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ILOG_H
#define ILOG_H

#include "ICore.h"

#include "event.h"

struct ILog : public ICore
{
	virtual ~ILog() {}

    virtual QString interfaceName() const { return "ILog"; }

    virtual void write(event* e) = 0;
};
// Q_DECLARE_INT

#endif // ILOG_H
