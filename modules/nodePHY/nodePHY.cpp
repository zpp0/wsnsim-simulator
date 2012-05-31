/**
 *
 * File: nodePHY.cpp
 * Description: low level node's software module
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "nodePHY.h"

bool NodePHY::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    // TODO: template
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    m_timer = (ITimer*)isimulator->getNodeInterface(this, m_parentNode, "ITimer");

    m_wakeUpPeriod = params["wakeUpPeriod"].toULong();
    
    return true;
}

void NodePHY::init()
{
    m_timer->start(m_wakeUpPeriod, "wakeUp");
}

void NodePHY::eventHandler(QString name, QVariantList params)
{
        // FIXME: ugly code
    if (name == "nodePowerUp")
        init();
    if (name == "timerInterrupt")
        if (params[1].toString() == "wakeUp")
            init();
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(nodePHY, NodePHY);
QT_END_NAMESPACE
