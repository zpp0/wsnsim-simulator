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

// Класс среды

class env : public QObject
{
    Q_OBJECT

public:
    // ~env();

    // реальное время в системе
    static VirtualTime time;

    static QList<IEnvironment*> modules;

    // // размеры среды по 3 измерениям
    // static EnvSize size;

    // // список зарегистрированных узлов
    // static QVector<node*> nodes;
    // // static QHash<node*, double*> nodesCoords;

private:

};

#endif // ENV_H
