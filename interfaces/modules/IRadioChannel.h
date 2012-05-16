/**
 *
 * IRadioChannel.h - Radio channel interface
 *
 **/

#ifndef IRADIOCHANNEL_H
#define IRADIOCHANNEL_H

#include <QtCore>

#include "IEnvironment.h"

#include "INode.h"

#include "types.h"

class IRadioChannel : public IEnvironment
{
public:
    IRadioChannel()
    {
        interfaceInfo.name = "IRadioChannel";
        interfaceInfo.events["ChangeLink"]["NodeID"] = "NodeID";
        interfaceInfo.events["ChangeLink"]["NodeID2"] = "NodeID2";
        interfaceInfo.events["newMessage"]["NodeID"] = "NodeID";
        interfaceInfo.events["newMessage"]["message"] = "byteArray";
        interfaceInfo.events["newMessage"]["RSSI"] = "double";
    }

    virtual void send(INode* node, byteArray message) = 0;

    virtual double aroundPower(INode* node) = 0;

};

#endif // IRADIOCHANNEL_H
