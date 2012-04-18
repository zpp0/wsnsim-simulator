/**
 *
 * File: node.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef NODE_H
#define NODE_H

#include "types.h"
#include "IModule.h"
#include "IHardware.h"
#include "ISoftware.h"

// FIXME: DELETE THIS
#include "ITimer.h"
#include "Irtx.h"

class Node : public QObject
{
    Q_OBJECT

public:
    Node(NodeID Id) : ID(Id) {}

    // ~node();
    IModule* getInterface(IModule* module, QString interface);

    // FIXME: THIS WILL BE DELETED!
    QMap<QString, IModule*> m_interfaces_TEMP;

    // FIXME: delete this
    // служебный ID узла
    NodeID ID;

    void init();

    static void timerInterruptHelper(void* object, QString type);

private:

    // // служебный ID узла
    // NodeID m_ID;

    IHardware* m_p_hardware;
    ISoftware* m_p_software;

    // FIXME: DELETE IT
    ITimer* m_timer;
    Irtx* m_rtx;

    void sendMessageCSMACA(byteArray message);
    VirtualTime CSMA_CA_delay();

    byteArray m_CSMA_CA_message;

    void inter(QString type);

    int BE;
    int NB;

    // 10 секунд
    static const quint64 sendMessagePeriod = 1000 * 1000 * 10;

    static const int ByteSendingTime =  32;
    static const int macMinBE = 3;
    static const int aMaxBE = 5;
    static const int macMaxCSMABackoffs = 4;
    static const int aUnitBackoffPeriod = 20;
    static const int pauseTime = aUnitBackoffPeriod * ByteSendingTime/2;

};

#endif // NODE_H
