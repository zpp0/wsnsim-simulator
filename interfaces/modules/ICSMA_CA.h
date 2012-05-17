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

        interfaceInfo.events["CSMA_delay"]["NodeID"] = "NodeID";
        interfaceInfo.events["CSMA_delay"]["BE"] = "uint";
        interfaceInfo.events["CSMA_delay"]["NB"] = "uint";
        interfaceInfo.events["CSMA_delay"]["delay"] = "uint";

        interfaceInfo.events["CSMA_begin"]["NodeID"] = "NodeID";
        interfaceInfo.events["CSMA_success"]["NodeID"] = "NodeID";
        interfaceInfo.events["CSMA_fail"]["NodeID"] = "NodeID";
    }

    virtual void sendMessage(byteArray message) = 0;
};

#endif // ICSMA_CA_H
