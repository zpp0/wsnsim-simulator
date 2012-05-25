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

        interfaceInfo.eventDescription["CSMA_begin"] = "Запущен алгоритм CSMA-CA";
        interfaceInfo.eventDescription["CSMA_delay"] = "Задержка алгоритма CSMA-CA. Аргументы - NE, BE и время ожидания в мкс";
        interfaceInfo.eventDescription["CSMA_success"] = "Успешное завершение алгоритма CSMA-CA";
        interfaceInfo.eventDescription["CSMA_fail"] = "Безуспешное завершение алгоритма CSMA-CA";
    }

    virtual void sendMessage(byteArray message) = 0;
};

#endif // ICSMA_CA_H
