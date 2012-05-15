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

class Timer : public ITimer
{
    Q_OBJECT
    Q_INTERFACES(ITimer)

public:
    Timer()
    {
        moduleInfo.name = "Timer";
        moduleInfo.version = "0.1";
        moduleInfo.description = "";
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

private:
    IEvent* m_event;
    IEnv* m_env;
    INode* m_parentNode;
};

#endif // TIMER_H
