/**
 *
 * scene.cpp
 *
 **/

#include "scene.h"

#include "INodesFactory.h"

bool moduleInit(ISimulator* isimulator, QMap<QString, QString> params);
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
    // m_size[2] = 1;

    // создаем узлы
    // запоминаем созданные узлы и их координаты в объекте среды

    // для всех параметров узлов из массива
    for (int i = 0; i < nodesNum; i++) {

        // создаем узел
        Node* nodeNew = new Node(i);

        // получаем координаты узла
        double* coords = new double[2];

        VirtualTime nodePowerUpTime;

        // иначе, генерируем их случайно
        for (int a = 0; a < 2; a++) {
            if (isRandomEnabled) {
                qsrand(QDateTime::currentDateTime().toTime_t() + a + (quint64)nodeNew);

                nodePowerUpTime = ((double)qrand() / RAND_MAX) * nodePowerUpTimeRange;
                coords[a] = ((double)qrand() / RAND_MAX) * m_size[a];
            }
            else {
                nodePowerUpTime = 100;
                coords[a] = 200;
            }
        }
        // пока они совпадают с уже существующими
        // } while (isSameCoords(coords) != -1);

        qDebug() << "nodeNew" << nodeNew->ID
                 << "coords" << coords[0] << coords[1]
                 << "time" << nodePowerUpTime;

        m_nodes += nodeNew;
        m_nodesCoords[nodeNew] = coords;

        nodePowerUp* event = new nodePowerUp();
        qDebug() << "event count" << event::count;
        event->time = nodePowerUpTime;
        event->eventNode = nodeNew->ID;
        event->coords[0] = coords[0];
        event->coords[1] = coords[1];

        Env::queue.insert(event);
    }

    // вычисляем расстояния между узлами
    foreach(Node* node1, m_nodes) {
        // вычисляем расстояния от этого узла до остальных
        foreach(Node* node2, m_nodes) {

            double* coord1 = coord(node1);
            double* coord2 = coord(node2);

            double distance = sqrt(pow((coord2[0]-coord1[0]), 2)
                                   + pow((coord2[1]-coord1[1]), 2));
            // + pow((coord2[2]-coord1[2]), 2));

            qDebug() << "distance node1" << node1->ID
                     << "node2" << node2->ID
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
