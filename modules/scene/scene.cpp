/**
 *
 * scene.cpp
 *
 **/

#include "scene.h"

bool moduleInit(ISimulator* isimulator, QMap<QString, QString> params);
{
    double x_size = X_SIZE-1;
    double y_size = Y_SIZE-1;
    
    int nodesNum = NODESNUM-1;

    VirtualTime nodePowerUpTimeRange = 100-1;

    bool isRandomEnabled = false;

    foreach (ModuleParam param, params) {
        if (param.name == "xSize")
            x_size = param.value.toDouble();
        if (param.name == "ySize")
            y_size = param.value.toDouble();
        if (param.name == "nodeNum")
            nodesNum = param.value.toInt();
        if (param.name == "nodePowerUpTimeRange")
            nodePowerUpTimeRange = param.value.toULong();
        if (param.name == "isRandomEnabled")
            isRandomEnabled = param.value.toInt();
    }

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

        // // если координаты не случайные, то получаем их из структуры параметров
        // if (nodes[i]->random == false) {
        //     // проверяем на совподение координат текущего узла с уже созданными
        //     int nodeIdWithSameCoord = isSameCoords(nodes[i]->coord);
        //     // если совпадений нет
        //     if (nodeIdWithSameCoord == -1)
        //         // получаем координаты нового узла из структуры параметров
        //         memmove(coords, nodes[i]->coord, sizeof(double) * 3);

        //     // если есть совпадение
        //     else {
        //         // пишем ошибку и выходим
        //         std::cerr << "Error: Nodes " << nodeIdWithSameCoord << " and " << i
        //                   << " have same coords: "  << nodes[i]->coord[0] << nodes[i]->coord[1] << nodes[i]->coord[2] << std::endl
        //                   << "Exit." << std::endl;

        //         exit(1);
        //     }
        // }
        
        VirtualTime nodePowerUpTime;
                
        // иначе, генерируем их случайно
        // else {
        // do {
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

    
        
        // const char* startUpTimeFuncName = nodeNew->getNodeType()->functionName(NODE_POWERON_TIME);

    //     quint64 startUpTime = 0;
        
    //     if (strcmp(startUpTimeFuncName, "") != 0) {

    //         QString randomSeedInit = "math.randomseed(" + QString::number(i) + ")";
    //         luaL_dostring(nodeNew->getLua(), randomSeedInit.toUtf8().constData());

    //         // вытаскиваем функцию процеесса lua 
    //         lua_getglobal(nodeNew->getLua(), startUpTimeFuncName);
            
    //         //вызов функции процесса
    //         if (lua_pcall(nodeNew->getLua(), 0, 1, 0) != 0)
    //             std::cerr << "Warning: node " << nodeNew->getMAC()
    //                       << " start up error " << std::endl;
            
    //         startUpTime = lua_tonumber(nodeNew->getLua(), -1);
    //         lua_pop(nodeNew->getLua(), 1);

    //         // if (startUpTime > m_maxSimulatorWorkTime)
    //         // startUpTime = 0;
    //     }
    //     qDebug() << "create node";
    //     // std::cerr << "create event NodeOn on time " << startUpTime << " node " << nodeNew << " id " << nodeNew->getMAC() << std::endl;

    //     nodeEventOn* event = new nodeEventOn(startUpTime, nodeNew, coords);
    //     insertEventQueue(event);
    // }

    // foreach (Node* node, m_nodes)
    //     node->init();

    // успешная инициализация
    return true;
}

int Scene::dimension()
{
    return 2;
}

double* Scene::coord(Node* node)
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

double Scene::distance(Node* node1, Node* node2)
{
    return m_distances[node1][node2];
}


// #include "moc_scene.cpp"

Q_EXPORT_PLUGIN(Scene);
// Q_EXPORT_PLUGIN2(sceneplugin, Scene);
