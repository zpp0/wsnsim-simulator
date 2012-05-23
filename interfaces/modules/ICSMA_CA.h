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

        interfaceInfo.events["CSMA_delay"]["NodeID"] = "uint16";
        interfaceInfo.events["CSMA_delay"]["BE"] = "uint8";
        interfaceInfo.events["CSMA_delay"]["NB"] = "uint8";
        interfaceInfo.events["CSMA_delay"]["delay"] = "uint64";

        interfaceInfo.events["CSMA_begin"]["NodeID"] = "uint16";
        interfaceInfo.events["CSMA_success"]["NodeID"] = "uint16";
        interfaceInfo.events["CSMA_fail"]["NodeID"] = "uint16";
    }

    virtual void sendMessage(byteArray message) = 0;
};

#endif // ICSMA_CA_H
