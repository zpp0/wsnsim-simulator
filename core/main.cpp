/**
 *
 * File: main.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QtCore/QCoreApplication>
#include <iostream>

#include "simulator.h"

int main(int argc, char *argv[])
{
    // qDebug() << "args" << argv[0] << argv[1] << argv[2];
    if (argc == 2) {
        Simulator sim(argv[1]);
        sim.eval();
    }
    else {
        std::cerr << "ERROR: Project file argument is out. Exit." << std::endl;
        std::cerr << "Use 'simulator projectFile.xml'" << std::endl;
    }
    
    return 0;
}
