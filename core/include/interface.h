/**
 *
 * File: interface.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtCore>

class Interface : public QObject
{
    Q_OBJECT

public:
    virtual QString interfaceName() const = 0;

};

#endif // INTERFACE_H
