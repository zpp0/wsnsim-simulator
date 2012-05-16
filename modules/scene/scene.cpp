/**
 *
 * scene.cpp
 *
 **/

#include "scene.h"

#include "INodesFactory.h"

#include "IEvent.h"

bool Scene::moduleInit(ISimulator* isimulator, QMap<QString, QString> params)
{
    double x_size = params["xSize"].toDouble();
    double y_size = params["ySize"].toDouble();
    int nodesNum = params["nodeNum"].toInt();
    VirtualTime nodePowerUpTimeRange = params["nodePowerUpTimeRange"].toULong();
    bool isRandomEnabled = params["isRandomEnabled"].toInt();

    qDebug("Scene params gotten:");
    qDebug("xsize: %f ysize: %f nodes: %i nodePowerUpTimeRange: %llu isRandomEnabled: %i",
           x_size, y_size, nodesNum, nodePowerUpTimeRange, isRandomEnabled);

    // размеры среды
    m_size[0] = x_size;
    m_size[1] = y_size;

    INodesFactory* factory = (INodesFactory*)isimulator->getCoreInterface(this, "INodesFactory");
    // создаем узлы
    // запоминаем созданные узлы и их координаты в объекте среды

    // для всех параметров узлов из массива
    for (int i = 0; i < nodesNum; i++) {


        // создаем узел
        INode* nodeNew = factory->create();

        // получаем координаты узла
        double* coords = new double[2];

        VirtualTime nodePowerUpTime;

        // иначе, генерируем их случайно
        // FIXME: coords cann't been the same
        for (int a = 0; a < 2; a++) {
            qsrand(QDateTime::currentDateTime().toTime_t() + a + (quint64)nodeNew);

            nodePowerUpTime = ((double)qrand() / RAND_MAX) * nodePowerUpTimeRange;
            coords[a] = ((double)qrand() / RAND_MAX) * m_size[a];
        }

        qDebug() << "nodeNew" << nodeNew->ID()
                 << "coords" << coords[0] << coords[1]
                 << "time" << nodePowerUpTime;

        m_nodes += nodeNew;
        m_nodesCoords[nodeNew] = coords;

        IEvent* event = (IEvent*)isimulator->getCoreInterface(this, "IEvent");

        event->post(this, "nodePowerUp", nodePowerUpTime,
                    QVariantList() << nodeNew->ID() << coords[0] << coords[1]);
    }

    // вычисляем расстояния между узлами
    foreach(INode* node1, m_nodes) {
        // вычисляем расстояния от этого узла до остальных
        foreach(INode* node2, m_nodes) {

            double* coord1 = coord(node1);
            double* coord2 = coord(node2);

            double distance = sqrt(pow((coord2[0]-coord1[0]), 2)
                                   + pow((coord2[1]-coord1[1]), 2));

            qDebug() << "distance node1" << node1->ID()
                     << "node2" << node2->ID()
                     << "is" << distance;

            // запоминаем расстояние
            m_distances[node1][node2] = distance;
        }
    }

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

double Scene::distance(INode* node1, INode* node2)
{
    return m_distances[node1][node2];
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(scene, Scene);
QT_END_NAMESPACE
