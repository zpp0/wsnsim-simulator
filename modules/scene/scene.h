/**
 *
 * scene.h
 *
 **/

#ifndef SCENE_H
#define SCENE_H

#include <QtCore>

#include "IScene.h"

class Scene : public QObject, public IScene
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    Scene()
    {
        moduleInfo.name = "Scene";
        moduleInfo.version = "0.1";
        moduleInfo.description = "Двухмерная сцена. Определяет положение узлов в пространстве";
        moduleInfo.exportInterface = "IScene";

        moduleInfo.importInterfaces += "INodesFactory";
        moduleInfo.importInterfaces += "INode";
        moduleInfo.importInterfaces += "IEvent";
        
        moduleInfo.params["xSize"] = "double";
        moduleInfo.params["ySize"] = "double";
        moduleInfo.params["nodeNum"] = "int";
        moduleInfo.params["nodePowerUpTimeRange"] = "ULong";

        moduleInfo.paramDescription["xSize"] = "Ширина сцены (double)";
        moduleInfo.paramDescription["ySize"] = "Высота сцены (double)";
        moduleInfo.paramDescription["nodeNum"] = "Количество узлов в сцене (uint16)";
        moduleInfo.paramDescription["nodePowerUpTimeRange"] = "Максимальное время включения узлов (uint64)";
    }

    /* virtual */ bool moduleInit(ISimulator* isimulator,
                                  QMap<QString, QString> params);

    /* virtual */ int dimension();
    /* virtual */ INode* node(NodeID nodeID);
    /* virtual */ double* coord(INode* node);
    /* virtual */ double* size();
    /* virtual */ double distance(INode* node1, INode* node2);

    /* virtual */ void eventHandler(QString eventName, QVariantList params){}
private:
    int isSameCoords(double coord[2]);

    double m_size[2];

    QVector<INode*> m_nodes;
    QHash<INode*, double*> m_nodesCoords;

    QMap<INode*, QMap<INode*, double> > m_distances;

};

#endif // SCENE_H
