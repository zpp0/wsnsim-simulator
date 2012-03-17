/**
 *
 * File: simulator.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <iostream>
#include <cassert>

#include "simulator.h"
#include "env.h"
#include "node.h"
#include "event.h"
#include "log.h"

#include "test-events.h"

simulator::simulator(QString projectFilePath)
{
    log::init(projectFilePath);
}

simulator::~simulator()
{
    log::uninit();
}

void simulator::loadProject (QString projectFileName)
{

}

void simulator::eval()
{
    // restrictions
    quint64 eventsNum = 100;
    quint16 nodesNum = 25;
    virtualTime maxTime = 86400000000;

    quint8 events = 10;
    quint64 eventsCount = 0;
    virtualTime time = 0;

    for (quint16 newNodeID = 0; newNodeID < nodesNum; newNodeID++) {

        qsrand(QDateTime::currentDateTime().toTime_t() + eventsCount);
        time += ((double)qrand() / RAND_MAX) * 1000000;

        double coords[3];
        for (int a = 0; a < 3; a++) {
            qsrand(QDateTime::currentDateTime().toTime_t() + a + time);
            coords[a] = ((double)qrand() / RAND_MAX) * 25;
        }

        nodeOn event;

        event.time = time;
        event.eventID = 0;
        event.eventNodeID = newNodeID;
        event.coords[0] = coords[0];
        event.coords[1] = coords[1];
        event.coords[2] = coords[2];
        
        // qDebug() << event.eventName() << event.time << event.eventNodeID << event.coords[0] << event.coords[1] << event.coords[2];

        event.record(log::logStream());
        
        eventsCount++;
    }

    while ((eventsCount < eventsNum)
           && (time < maxTime)) {
        nodeID randomNodeID = ((double)qrand() / RAND_MAX) * nodesNum;
        eventID randomEventID = ((double)qrand() / RAND_MAX) * events;

        switch(randomEventID) {
        case 0: case 1:
            break;
        case 2:
        {
            changeChannel event;

            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            quint16 randomChannel = ((double)qrand() / RAND_MAX) * 11;
            event.newChannel = randomChannel;
            
            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.newChannel;

            event.record(log::logStream());

            break;
        }
        case 3:
        {
            rx event;

            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            QString message;
            message = "1" + QString::number(time) + QString::number(randomNodeID - 1);
            event.message.length = message.size();
            event.message.array = message.toAscii();
            event.RSSI = -45.0;

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.message.length << event.message.array << event.RSSI;
            
            event.record(log::logStream());
            break;            
        }

        case 4:
        {
            tx event;

            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            QString message;
            message = "1" + QString::number(time) + QString::number(randomNodeID + 1);
            event.message.length = message.size();
            event.message.array = message.toAscii();
            event.TXPower = 0;

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.message.length << event.message.array << event.TXPower;
            
            event.record(log::logStream());
            break;            
        }

        case 5:
        {            
            timer event;

            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;

            // qDebug() << event.eventName() << event.time << event.eventNodeID;
            
            event.record(log::logStream());
            break;
        }


        case 6:
        {            
            measure event;

            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            QString sensorName = "temperature";
            event.sensorName.length = sensorName.size();
            event.sensorName.string = sensorName.toUtf8();
            event.value = 25.0;

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.sensorName.length << event.sensorName.string << event.value;

            event.record(log::logStream());
            break;
        }

        case 7:
        {
            double coords[3];
            for (int a = 0; a < 3; a++) {
                qsrand(QDateTime::currentDateTime().toTime_t() + a + time);
                coords[a] = ((double)qrand() / RAND_MAX) * 25;
            }

            move event;
            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            event.coords[0] = coords[0];
            event.coords[1] = coords[1];
            event.coords[2] = coords[2];

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.coords[0] << event.coords[1] << event.coords[2];

            event.record(log::logStream());
            break;
        }

        case 8:
        {
            logtime event;
            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            event.localTime = time - 15;

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.localTime;
            
            event.record(log::logStream());
            break;
        }

        case 9:
        {
            changelink event;
            event.time = time;
            event.eventID = randomEventID;
            event.eventNodeID = randomNodeID;
            event.eventNodeID2 = randomNodeID + 2;

            // qDebug() << event.eventName() << event.time << event.eventNodeID << event.eventNodeID2;
            
            event.record(log::logStream());
            break;
        }
        
        }

        time += ((double)qrand() / RAND_MAX) * 1000;
        eventsCount++;

    }
           
    qDebug() << "time" << time << "events" << eventsCount;
}
