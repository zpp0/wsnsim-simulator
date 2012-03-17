/**
 *
 * test-events.h
 *
 **/

#ifndef TESTEVENTS_H
#define TESTEVENTS_H

#include <QtCore>

#include "types.h"
#include "nodeEvent.h"
// #include "loggableEvent.h"

// class SFD_RX_Up : public nodeEvent, public recordedEvent, public processedEvent
// {
//     virtual QString eventName() { return "rx"; }
// };

struct byteArray
{
    quint8 length;
    QByteArray array;
};

struct string
{
    quint16 length;
    QByteArray string;
};

class nodeOn : public nodeEvent
{
public:
    virtual QString eventName() const { return "node on"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << coords[0] << coords[1] << coords[2];
    }

    double coords[3];
};

class nodeOff : public nodeEvent
{
public:
    virtual QString eventName() const { return "node off"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID;
    }
};

class changeChannel : public nodeEvent
{
public:
    virtual QString eventName() const { return "change channel"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << newChannel;
    }

    quint16 newChannel;
};

class rx : public nodeEvent
{
public:
    virtual QString eventName() const { return "rx"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << message.length;
        stream.writeRawData(message.array.constData(), message.length);
        stream << RSSI;
    }

    byteArray message;
    double RSSI;
};

class tx : public nodeEvent
{
public:
    virtual QString eventName() const { return "tx"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << message.length;
        stream.writeRawData(message.array.constData(), message.length);
        stream << TXPower;
    }

    byteArray message;
    double TXPower;
};


class timer : public nodeEvent
{
public:
    virtual QString eventName() const { return "timer"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID;
    }
};

class measure : public nodeEvent
{
public:
    virtual QString eventName() const { return "measure"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID;

        stream << sensorName.length;
        stream.writeRawData(sensorName.string.constData(), sensorName.length);

        stream << value;
    }

    string sensorName;
    double value;
};

class move : public nodeEvent
{
public:
    virtual QString eventName() const { return "move"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << coords[0] << coords[1] << coords[2];
    }

    double coords[3];
};

class logtime : public nodeEvent
{
public:
    virtual QString eventName() const { return "logtime"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << localTime;
    }

    virtualTime localTime;
};


class changelink : public nodeEvent
{
public:
    virtual QString eventName() const { return "changelink"; }

    virtual void record(QDataStream& stream)
    { 
        stream << time << eventID << eventNodeID << eventNodeID2;
    }

    nodeID eventNodeID2;
};

#endif // TESTEVENTS_H
