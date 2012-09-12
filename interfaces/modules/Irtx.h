/**
 *
 * Irtx.h - RTX interface
 *
 **/

#ifndef IRTX_H
#define IRTX_H

#include <QtCore>

#include "IHardware.h"

enum rtxState
{
    rtxState_OFF,
    rtxState_Free,
    rtxState_RXON,
    rtxState_TXON
};

class Irtx : public IHardware
{
public:
    ~Irtx() {}
    Irtx()
    {
        interfaceInfo.name = "Irtx";

        interfaceInfo.events["SFD_RX_Up"] << qMakePair(QString("NodeID"), QString("uint16"))
                                          << qMakePair(QString("message"), QString("ByteArray"))
                                          << qMakePair(QString("RSSI"), QString("double"));

        interfaceInfo.events["SFD_RX_Down"] << qMakePair(QString("NodeID"), QString("uint16"))
                                            << qMakePair(QString("message"), QString("ByteArray"));

        interfaceInfo.events["SFD_TX_Up"] << qMakePair(QString("NodeID"), QString("uint16"))
                                          << qMakePair(QString("message"), QString("ByteArray"))
                                          << qMakePair(QString("TXPower"), QString("double"));

        interfaceInfo.events["SFD_TX_Down"] << qMakePair(QString("NodeID"), QString("uint16"));

        interfaceInfo.events["Collision"] << qMakePair(QString("NodeID"), QString("uint16"));

        interfaceInfo.events["CCATest"] << qMakePair(QString("NodeID"), QString("uint16"))
                                        << qMakePair(QString("State"), QString("uint16"));

        interfaceInfo.events["message_dropped"] << qMakePair(QString("NodeID"), QString("uint16"))
                                                << qMakePair(QString("message"), QString("ByteArray"));

        interfaceInfo.events["MessageReceived"] << qMakePair(QString("NodeID"), QString("uint16"))
                                                << qMakePair(QString("message"), QString("ByteArray"));

        interfaceInfo.events["MessageSent"] << qMakePair(QString("NodeID"), QString("uint16"));

        interfaceInfo.eventDescription["SFD_RX_Up"] = "Прерывание по линии SFD по началу приема сообщения";
        interfaceInfo.eventDescription["SFD_RX_Down"] = "Прерывание по линии SFD по окончанию приема сообщения";
        interfaceInfo.eventDescription["SFD_TX_Up"] = "Прерывание по линии SFD по началу передачи сообщения";
        interfaceInfo.eventDescription["SFD_TX_Down"] = "Прерывание по линии SFD по окончанию передачи сообщения";
        interfaceInfo.eventDescription["Collision"] = "При приеме сообщения произошла коллизия";
        interfaceInfo.eventDescription["CCATest"] = "Проверка состояния канала";
        interfaceInfo.eventDescription["message_dropped"] = "Узел прекратил прием сообщения. Либо произошла коллизия, либо пришло сообщение с большей мощностью";
    }

    virtual quint64 getLongAddr() = 0;

    virtual void setTXPower(int power) = 0;
    virtual void setChannel(int newChannel) = 0;
    virtual void setPower(bool on) = 0;
    virtual void setCCAThreshold(int threshold) = 0;

    virtual void startTX(byteArray message) = 0;
    virtual void startTX(byteArray message, void (*handler)()) = 0;
    virtual void waitTXEnd() = 0;
    virtual bool CCA() = 0;

    virtual int TXPower() = 0;
    virtual int RXSensivity() const = 0;

    virtual rtxState state() = 0;
};

#endif // IRTX_H
