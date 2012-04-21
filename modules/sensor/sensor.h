/**
 *
 * sensor.h
 *
 **/

#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore>

#include "ISensor.h"
#include "IField.h"

// FIXME: delete this
#include "IScene.h"

#include "node.h"

class Sensor : public ISensor
{
    Q_OBJECT
	Q_INTERFACES(ISensor)

public:
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QString deviceName() const;
    virtual QList<InterruptHandler> interrupts();

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    // FIXME: DELETEA!!!1
    void setParentNode(Node* parentNode) { m_parentNode = parentNode; }

    virtual double measure();

private:

    IField* m_field;
    IScene* m_scene;

    Node* m_parentNode;

};
// Q_EXPORT_PLUGIN(Sensor);

#endif // SENSOR_H
