/**
 *
 * IRadioChannel.h - Radio channel interface
 *
 **/

#ifndef IRADIOCHANNEL_H
#define IRADIOCHANNEL_H

#include <QtCore>

class IRadioChannel
{
public:
    virtual void send(byteArray& message) = 0;

    virtual byteArray* listen() = 0;
    
    // TODO: need RSSI check

};

#endif // IRADIOCHANNEL_H
