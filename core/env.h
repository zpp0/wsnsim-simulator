/**
 *
 * File: env.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef ENV_H
#define ENV_H

#include <QList>

#include "IEnvironment.h"
#include "types.h"
#include "eventQueue.h"

// Класс среды

class Env : public QObject
{
    Q_OBJECT

public:
    // ~env();

    // реальное время в системе
    static VirtualTime time;

    static IModule* getInterface(IModule* module, QString interface);

    // static QList<IEnvironment*> modules;

    // // размеры среды по 3 измерениям
    // static EnvSize size;

    // // список зарегистрированных узлов
    // static QVector<node*> nodes;
    // // // static QHash<node*, double*> nodesCoords;

    // FIXME: THIS WILL BE DELETED!
    static QMap<QString, IModule*> m_interfaces_TEMP;

    // FIXME: THIS WILL BW DELETED
    static eventQueue queue;

private:

};

#endif // ENV_H
