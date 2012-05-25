/**
 *
 * File: csma_ca.h
 * Description: CSMA CA module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef CSMA_CA_H
#define CSMA_CA_H

#include <QtCore>

#include "ICSMA_CA.h"

#include "Irtx.h"
#include "ITimer.h"
#include "INode.h"
#include "IEvent.h"

class CSMA_CA : public QObject, public ICSMA_CA
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    CSMA_CA()
    {
        moduleInfo.name = "CSMA_CA";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Программа алгоритма CSMA-CA";
        moduleInfo.exportInterface = "ICSMA_CA";

        moduleInfo.importInterfaces += "Irtx";
        moduleInfo.importInterfaces += "ITimer";
        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IEvent";

        moduleInfo.handledEvents += "timerInterrupt";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void sendMessage(byteArray message);

    /* virtual */ void eventHandler(QString name, QVariantList params);

private:

    void timerInterrupt();
    VirtualTime delay();

    Irtx* m_rtx;
    ITimer* m_timer;

    INode* m_parentNode;

    IEvent* m_event;

    byteArray m_message;

    int m_BE;
    int m_NB;
};

#endif // CSMA_CA_H
