/**
 *
 * timer.h
 *
 **/

#ifndef TIMER_H
#define TIMER_H

#include <QtCore>

#include "ITimer.h"

#include "node.h"

class Timer : public ITimer
{
    Q_OBJECT
	Q_INTERFACES(ITimer)

public:
    // TODO: DELETE THIS
    // Timer(NodeID parentID) : m_parentID(parent);
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    virtual QString deviceName() const;
    virtual QList<InterruptHandler> interrupts();

    void setParentNode(Node* parent) { m_parent = parent; }

    virtual VirtualTime getValue();
    virtual void setValue(VirtualTime newTime);
    virtual void start(VirtualTime timeout, void (*timerInterrupt)(void*, QString), void* object, QString type);
    virtual void stop(InterruptHandler timerInterrupt);

private:
    // TODO: DELETE THIS
    Node* m_parent;

};
// Q_EXPORT_PLUGIN(Timer);

#endif // TIMER_H
