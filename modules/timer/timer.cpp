/**
 *
 * timer.cpp
 *
 **/

#include "timer.h"

bool Timer::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    m_parentNode = (INode*)isimulator->getCoreInterface(this, "INode");
    m_env = (IEnv*)isimulator->getCoreInterface(this, "IEnv");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");

    return true;
}

VirtualTime Timer::getValue()
{
    return m_env->globalTime();
}

void Timer::setValue(VirtualTime newTime)
{
    // TODO: do
}

void Timer::start(VirtualTime timeout, QString type)
{
    m_event->post(this, "timerInterrupt", timeout,
                  QVariantList() << m_parentNode->ID() << type);
}

void Timer::stop(QString type)
{
    // TODO: do
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(timer, Timer);
QT_END_NAMESPACE
