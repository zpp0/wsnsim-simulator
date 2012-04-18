/**
 *
 * timer.cpp
 *
 **/

#include "timer.h"

#include "env.h"

QString Timer::moduleName() const
{
    return "Timer";
}

QString Timer::moduleVersion() const
{
    return "0.1";
}

QString Timer::moduleDescription() const
{
    return "Модуль таймера";
}

bool Timer::moduleInit(QList<ModuleParam> params)
{
    return true;
}

QList<QString> Timer::moduleExportInterfaces() const
{
    QList<QString> tmp;
    return tmp;
}

QList<QString> Timer::moduleImportInterfaces() const
{
    QList<QString> tmp;
    return tmp;
}

QString Timer::deviceName() const
{
    return "Timer";
}

QList<InterruptHandler> Timer::interrupts()
{
    QList<InterruptHandler> tmp;
    return tmp;
}

VirtualTime Timer::getValue()
{
    return Env::time;
}

void Timer::setValue(VirtualTime newTime)
{
    // TODO: do
}

void Timer::start(VirtualTime timeout, void (*interrupt)(void*, QString), void* object, QString type)
{
    qDebug() << "timer start at time" << getValue() + timeout;
    timerInterrupt* Interrupt = new timerInterrupt();
    qDebug() << "event count" << event::count;
    // qDebug() << "time" << getValue() + timeout;
    Interrupt->time = getValue() + timeout;
    Interrupt->recordable = true;
    // qDebug() << "nodeID " << m_parent->ID;
    Interrupt->eventNode = m_parent->ID;
    Interrupt->handler = interrupt;
    Interrupt->object = object;
    Interrupt->type = type;
    Env::queue.insert(Interrupt);
}

void Timer::stop(InterruptHandler timerInterrupt)
{
    // TODO: do
}

quint64 ITimer::timerInterrupt::count = 0;
