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
        interfaceInfo.events["ChangeLink"] << qMakePair(QString("NodeID"), QString("uint16"))
                                           << qMakePair(QString("NodeID2"), QString("uint16"));

        interfaceInfo.events["newMessageInChannel"] << qMakePair(QString("NodeID"), QString("uint16"))
                                                    << qMakePair(QString("message"), QString("ByteArray"))
                                                    << qMakePair(QString("RSSI"), QString("double"));

        interfaceInfo.eventDescription["newMessageInChannel"] = "В канале узла появилось новое сообщение";
        interfaceInfo.eventDescription["ChangeLink"] = "Изменение матрицы слышимости";
    }

    virtual void send(INode* node, byteArray message) = 0;

    virtual double aroundPower(INode* node) = 0;

    virtual bool isNetworkConnected(QVector<INode*> nodes) = 0;

};

#endif // IRADIOCHANNEL_H
