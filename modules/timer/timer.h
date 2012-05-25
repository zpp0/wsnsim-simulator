/**
 *
 * timer.h
 *
 **/

#ifndef TIMER_H
#define TIMER_H

#include <QtCore>

#include "ITimer.h"

#include "INode.h"
#include "IEnv.h"
#include "IEvent.h"

class Timer : public QObject, public ITimer
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    Timer()
    {
        moduleInfo.name = "Timer";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Модель точных часов";
        moduleInfo.exportInterface = "ITimer";

        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IEnv";
        moduleInfo.importInterfaces += "IEvent";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ VirtualTime getValue();
    /* virtual */ void setValue(VirtualTime newTime);
    /* virtual */ void start(VirtualTime timeout, QString type);
    /* virtual */ void stop(QString type);

    /* virtual */ void eventHandler(QString eventName, QVariantList params) {}
    
private:
    IEvent* m_event;
    IEnv* m_env;
    INode* m_parentNode;
};

#endif // TIMER_H
