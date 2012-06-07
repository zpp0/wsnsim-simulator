/**
 *
 * File: ICTP.h
 * Description: command transfer protocol interface
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef ICTP_H
#define ICTP_H

#include <QtCore>

#include "ISoftware.h"

class ICTP : public ISoftware
{
public:
    ~ICTP() {}
    ICTP()
    {
        interfaceInfo.name = "ICTP";

        interfaceInfo.events["measureRequestSent"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("senderMACAddr"), QString("uint64"))
            << qMakePair(QString("seqNum"), QString("uint16"));

        interfaceInfo.events["measureRequestReceived"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("senderMACAddr"), QString("uint64"))
            << qMakePair(QString("seqNum"), QString("uint16"));

        interfaceInfo.events["measureAckSent"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("listenerMACAddr"), QString("uint64"))
            << qMakePair(QString("senderMACAddr"), QString("uint64"))
            << qMakePair(QString("seqNum"), QString("uint16"))
            << qMakePair(QString("value"), QString("double"));

        interfaceInfo.events["measureAckReceived"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("senderMACAddr"), QString("uint64"))
            << qMakePair(QString("seqNum"), QString("uint16"))
            << qMakePair(QString("value"), QString("double"));

        interfaceInfo.events["measureSentUART"]
            << qMakePair(QString("NodeID"), QString("uint16"))
            << qMakePair(QString("senderMACAddr"), QString("uint64"))
            << qMakePair(QString("seqNum"), QString("uint16"))
            << qMakePair(QString("value"), QString("double"));

    }

    // /* virtual */ void measureRequestSend() = 0;
    // /* virtual */ void measureRequestReceived() = 0;
    // /* virtual */ void measureStarted() = 0;
    // /* virtual */ void measureAckSended() = 0;
    // /* virtual */ void measureAckReceived() = 0;
};

#endif // ICTP_H
