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

void log::init(QString logFilePath)
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
    
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
}

void log::uninit()
{
    m_log->close();
}

void log::writeLog(loggableEvent* event)
{
    (*m_logStream) << event->time << events[event->eventName()];
    event->record(*m_logStream);
}
