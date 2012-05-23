/**
 *
 * File: ICSMA_CA.h
 * Description: CSMA CA interface
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef ICSMA_CA_H
#define ICSMA_CA_H

#include <QtCore>

#include "ISoftware.h"

class ICSMA_CA : public ISoftware
{
public:
    ~ICSMA_CA() {}
    ICSMA_CA()
    {
        interfaceInfo.name = "ICSMA_CA";

        interfaceInfo.events["CSMA_delay"] << qMakePair(QString("NodeID"), QString("uint16"))
                                           << qMakePair(QString("BE"), QString("int32"))
                                           << qMakePair(QString("NB"), QString("int32"))
                                           << qMakePair(QString("delay"), QString("uint64"));

        interfaceInfo.events["CSMA_begin"] << qMakePair(QString("NodeID"), QString("uint16"));
        interfaceInfo.events["CSMA_success"] << qMakePair(QString("NodeID"), QString("uint16"));
        interfaceInfo.events["CSMA_fail"] << qMakePair(QString("NodeID"), QString("uint16"));
    }

    virtual void sendMessage(byteArray message) = 0;
};

#endif // ICSMA_CA_H
