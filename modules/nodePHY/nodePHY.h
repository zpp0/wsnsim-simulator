/**
 *
 * File: nodePHY.h
 * Description: low level node's software module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef NODEPHY_H
#define NODEPHY_H

#include <QtCore>

#include "INodeInit.h"

#include "INode.h"
#include "Irtx.h"
#include "ITimer.h"
#include "ICSMA_CA.h"

class NodePHY : public QObject, public INodePHY
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    NodePHY()
    {
        moduleInfo.name = "NodePHY";
        moduleInfo.version = "0.1";
        moduleInfo.description = "";
        moduleInfo.exportInterface = "INodePHY";

        moduleInfo.importInterfaces += "Irtx";
        moduleInfo.importInterfaces += "ICSMA_CA";
        moduleInfo.importInterfaces += "ITimer";
        moduleInfo.importInterfaces += "INode";

        moduleInfo.params["sendMessagePeriod"] = "quint64";
        
        moduleInfo.handledEvents += "nodePowerUp";
        moduleInfo.handledEvents += "timerInterrupt";
        moduleInfo.handledEvents += "CSMA_success";
        moduleInfo.handledEvents += "CSMA_fail";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void init();

    /* virtual */ void eventHandler(QString name, QVariantList params);

private:

    VirtualTime m_sendMessagePeriod;

    void trySendMessage();
    
    INode* m_parentNode;
    
    Irtx* m_rtx;
    ITimer* m_timer;
    ICSMA_CA* m_csma;
};

#endif // NODEPHY_H
