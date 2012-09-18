/**
 *
 * scene.cpp
 *
 **/

#include "scene.h"

bool Scene::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    double x_size = params["xSize"].toDouble();
    double y_size = params["ySize"].toDouble();
    int nodesNum = params["nodeNum"].toInt();
    VirtualTime nodePowerUpTimeRange = params["nodePowerUpTimeRange"].toULong();
    bool connectivity = params["Network connectivity"].toInt();

    qDebug("Scene params gotten:");
    qDebug("xsize: %f ysize: %f nodes: %i nodePowerUpTimeRange: %llu",
           x_size, y_size, nodesNum, nodePowerUpTimeRange);

    // размеры среды
    m_size[0] = x_size;
    m_size[1] = y_size;

    m_nodesNum = nodesNum;
    m_nodePowerUpTimeRange = nodePowerUpTimeRange;
    m_connectivity = connectivity;
    
    m_factory = (INodesFactory*)isimulator->getCoreInterface(this, "INodesFactory");
    m_event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");
    
    if (m_connectivity)
        m_channel = (IRadioChannel*)isimulator->getEnvInterface(this, "IRadioChannel");

    m_event->post(this, "creatingScene", 0, QVariantList());
    
    // успешная инициализация
    return true;
}

int Scene::dimension()
{
    return 2;
}

INode* Scene::node(NodeID nodeID)
{
    foreach(INode* node, m_nodes)
        if (node->ID() == nodeID)
            return node;
}

double* Scene::coord(INode* node)
{
    return m_nodesCoords[node];
}

double* Scene::size()
{
    return m_size;
}

int Scene::isSameCoords(double coord[3])
{
    // QList<node*> nodes = m_h_nodes.values();
    for (int i = 0; i < m_nodes.size(); i++)
        // проверяем координаты a-го узла с новым
        if ((m_nodesCoords[m_nodes[i]][0] == coord[0])
            && (m_nodesCoords[m_nodes[i]][1] == coord[1]))
            // && (m_h_nodesCoords[m_nodes[i]][2] == coord[2]))
            return i;
    return -1;
}

void Scene::generateCoords(double* coord)
{
    // иначе, генерируем их случайно
    // FIXME: coords cann't been the same
    for (int i = 0; i < 2; i++)
        coord[i] = ((double)qrand() / RAND_MAX) * m_size[i];
}

void Scene::calculateDistances()
{
    // вычисляем расстояния между узлами
    foreach(INode* node1, m_nodes) {
        // вычисляем расстояния от этого узла до остальных
        foreach(INode* node2, m_nodes) {

            double* coord1 = m_nodesCoords[node1];
            double* coord2 = m_nodesCoords[node2];

            double distance = sqrt(pow((coord2[0]-coord1[0]), 2)
                                   + pow((coord2[1]-coord1[1]), 2));

            qDebug() << "distance node1" << node1->ID()
                     << "node2" << node2->ID()
                     << "is" << distance;

            // запоминаем расстояние
            m_distances[node1][node2] = distance;
        }
    }
}

double Scene::distance(INode* node1, INode* node2)
{
    return m_distances[node1][node2];
}

void Scene::eventHandler(QString eventName, QVariantList params)
{
    if (eventName == "creatingScene") {

        // создаем узлы
        // запоминаем созданные узлы и их координаты в объекте среды

        // для всех параметров узлов из массива
        for (int i = 0; i < m_nodesNum; i++) {

            // создаем узел
            INode* nodeNew = m_factory->create();

            // получаем координаты узла
            double* coords = new double[2];

            m_nodes += nodeNew;
            m_nodesCoords[nodeNew] = coords;
        }

        if (m_connectivity) {

            do {
                foreach (INode* node, m_nodes)
                    generateCoords(m_nodesCoords[node]);

                calculateDistances();

                qDebug() << "m_nodes size()" << m_nodes.size();
            } while (!m_channel->isNetworkConnected(m_nodes));
        }
        else {
            foreach (INode* node, m_nodes)
                generateCoords(m_nodesCoords[node]);
            calculateDistances();
        }

        for (int i = 0; i < m_nodesNum; i++) {

            VirtualTime nodePowerUpTime = ((double)qrand() / RAND_MAX) * m_nodePowerUpTimeRange;

            qDebug() << "nodeNew" << m_nodes[i]->ID()
                     << "coords" << m_nodesCoords[m_nodes[i]][0] << m_nodesCoords[m_nodes[i]][1]
                     << "time" << nodePowerUpTime;

            m_event->post(this, "nodePowerUp", nodePowerUpTime,
                          QVariantList() << m_nodes[i]->ID() << m_nodesCoords[m_nodes[i]][0] << m_nodesCoords[m_nodes[i]][1]);
        }
    }
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(scene, Scene);
QT_END_NAMESPACE
