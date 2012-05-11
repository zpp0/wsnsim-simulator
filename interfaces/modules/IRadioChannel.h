/**
 *
 * IRadioChannel.h - Radio channel interface
 *
 **/

#ifndef IRADIOCHANNEL_H
#define IRADIOCHANNEL_H

#include <QtCore>

#include "IEnvironment.h"

#include "types.h"
#include "node.h"

#include "nodeEvent.h"
#include "loggableEvent.h"

class IRadioChannel : public IEnvironment, public QObject
{
    Q_OBJECT
public:
    IRadioChannel()
    {
        interfaceInfo.name = "IRadioChannel";
        interfaceInfo.events["ChangeLink"]["NodeID"] = "NodeID";
        interfaceInfo.events["ChangeLink"]["NodeID2"] = "NodeID2";
    }

    virtual void send(Node* node, byteArray message) = 0;

    virtual byteArray listen(Node* node) = 0;

    virtual double aroundPower(Node* node) = 0;

};
Q_DECLARE_INTERFACE(IRadioChannel,
                    "simulator.IRadioChannel/0.1")

#endif // IRADIOCHANNEL_H
