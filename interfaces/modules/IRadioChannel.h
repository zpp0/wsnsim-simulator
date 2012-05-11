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

class IRadioChannel : public IEnvironment
{
public:
    virtual void send(Node* node, byteArray message) = 0;

    virtual byteArray listen(Node* node) = 0;

    virtual double aroundPower(Node* node) = 0;

    class ChangeLink : public nodeEvent
// , public loggableEvent
    {
    public:
        
        ChangeLink() { count++; }

        static quint64 count;

        NodeID node2;
        virtual QString eventName() const { return "ChangeLink"; }
        virtual void record(QDataStream& stream) { stream << eventNode << node2; }
        virtual void process() {}

    };

};
Q_DECLARE_INTERFACE(IRadioChannel,
                    "simulator.IRadioChannel/0.1")

#endif // IRADIOCHANNEL_H
