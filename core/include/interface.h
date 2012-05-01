/**
 *
 * File: interface.h
 * Description: Simulator interface bace class
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtCore>

#include "types.h"

class Interface : public QObject
{
    Q_OBJECT
public:
    virtual ~Interface() {};
    
    InterfaceInfo interfaceInfo;
};

#endif // INTERFACE_H
