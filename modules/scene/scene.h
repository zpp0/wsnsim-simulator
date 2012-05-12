/**
 *
 * scene.h
 *
 **/

#ifndef SCENE_H
#define SCENE_H

#include <QtCore>

#include "IScene.h"

class Scene : public IScene
{
    Q_OBJECT
    Q_INTERFACES(IScene)

public:
    Scene()
    {
        moduleInfo.name = "Scene";
        moduleInfo.version = "0.1";
        moduleInfo.description = "";
        moduleInfo.exportInterface = "IRadioChannel";
        moduleInfo.importInterfaces = QList("INodesFactory", "INode");
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ int dimension();
    /* virtual */ double* coord(INode* node);
    /* virtual */ double* size();
    /* virtual */ double distance(INode* node1, INode* node2);

    /* virtual */ quint16 nodesCount();

private:
    int isSameCoords(double coord[2]);

    double m_size[2];

    QVector<INode*> m_nodes;
    QHash<INode*, double*> m_nodesCoords;

    QMap<INode*, QMap<INode*, double> > m_distances;

};

#endif // SCENE_H
