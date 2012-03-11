/**
 *
 * File: node.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef NODE_H
#define NODE_H

#include "types.h"
#include "IHardware.h"
#include "ISoftware.h"

class node : public QObject
{
    Q_OBJECT

public:
    node(NodeID ID, IHardware* hardware, ISoftware* software)
        : m_ID(ID), m_p_hardware(hardware), m_p_software(software) {}

    // ~node();

private:
    // служебный ID узла
    NodeID m_ID;

    IHardware* m_p_hardware;
    ISoftware* m_p_software;
};

#endif // NODE_H
