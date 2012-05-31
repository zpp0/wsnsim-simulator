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
#include "ITimer.h"

class NodePHY : public QObject, public INodePHY
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    NodePHY()
    {
        moduleInfo.name = "NodePHY";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Программа инициализации узла";
        moduleInfo.exportInterface = "INodePHY";

        moduleInfo.importInterfaces += "ITimer";
        moduleInfo.importInterfaces += "INode";

        moduleInfo.params["wakeUpPeriod"] = "quint64";

        moduleInfo.paramDescription["wakeUpPeriod"] = "Период пробуждения узла в мкс (uint64)";
        
        moduleInfo.handledEvents += "nodePowerUp";
        moduleInfo.handledEvents += "timerInterrupt";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ void init();

    /* virtual */ void eventHandler(QString name, QVariantList params);

private:

    VirtualTime m_wakeUpPeriod;

    INode* m_parentNode;
    ITimer* m_timer;
};

#endif // NODEPHY_H
