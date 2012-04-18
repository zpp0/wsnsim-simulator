/**
 *
 * scene.h
 *
 **/

#ifndef SCENE_H
#define SCENE_H

#include <QtCore>

#include "IScene.h"

#include "node.h"

class Scene : public IScene
{
    Q_OBJECT
	Q_INTERFACES(IScene)

public:
    virtual QString moduleName() const;
	virtual QString moduleVersion() const;
    virtual QString moduleDescription() const;

	virtual bool moduleInit(QList<ModuleParam> params);

    virtual QList<QString> moduleExportInterfaces() const;
    virtual QList<QString> moduleImportInterfaces() const;

    virtual int dimension();
    virtual double* coord(Node* node);
    virtual double* size();
    virtual double distance(Node* node1, Node* node2);

    virtual quint16 nodesCount();
    
    virtual QVector<Node*> nodes();

private:
    int isSameCoords(double coord[2]);
    
    double m_size[2];
    
    QVector<Node*> m_nodes;
    QHash<Node*, double*> m_nodesCoords;

    QMap<Node*, QMap<Node*, double> > m_distances;

};

#endif // SCENE_H
