/**
 *
 * File: log.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QDataStream>

#include <iostream>

#include "log.h"

QFile* log::m_log;
QDataStream* log::m_logStream;
QHash<QString, EventID> log::events;

void log::init(QString logFilePath, Events eventsLists)
{
    // qDebug() << "log path is" << currentProjectPath;
    
    //для начала, удалим файл логов для того, чтобы создать новый
    // удаляем старый файл лога
    m_log = new QFile(logFilePath);
    m_log->remove();

    if (!m_log->open(QIODevice::Append)) {
        std::cerr << "Error: cannot open log file " << logFilePath.toStdString() << std::endl;
        exit(1);
    }

    m_logStream = new QDataStream(m_log);

    // foreach (QList<EventParams> eventsList, eventsLists) {
    foreach (EventParams params, eventsLists.systemEvents) {
        QString eventName = params.eventInfo["name"];
        EventID eventID = params.eventInfo["ID"].toUInt();
        events[eventName] = eventID;
    }
    // }

    // events["timerInterrupt"] = 0;
    // events["nodePowerUp"] = 1;
    // events["SFD_RX_Up"] = 2;
    // events["SFD_RX_Down"] = 3;
    // events["SFD_TX_Up"] = 4;
    // events["SFD_TX_Down"] = 5;
    // events["Collision"] = 6;
    // events["CCATest"] = 7;
    // events["ChangeLink"] = 8;
    // // events["timerInterrupt"] = 0;
    // // events[""]
    
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
}

void log::uninit()
{
    m_log->close();
}

// void log::writeLog(loggableEvent* event)
void log::writeLog(event* event)
{
    qDebug() << "write event in log" << events[event->eventName()] << event->eventName();
    // qDebug() << "write std data" << event;
    (*m_logStream) << event->time << events[event->eventName()];
    // (*m_logStream) << event->time;
    // qDebug() << "write event data" << event;
    event->record(*m_logStream);
}
