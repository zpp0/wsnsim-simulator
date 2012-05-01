/**
 *
 * File: IEvent.h
 * Description: Simulator events interface
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef IEVENT_H
#define IEVENT_H

#include "ICore.h"

struct IEvent : public ICore
{
    IEvent() { interfaceInfo.name = "IEvent"; }
    
    virtual void create(IModule* author, QString name, VirtualTime time, QVariantList params) = 0;
};
// Q_DECLARE_INT

#endif // IEVENT_H
