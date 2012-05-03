/**
 *
 * File: log.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QDataStream>

#include <iostream>

#include "log.h"

QFile* Log::m_log;
QDataStream* Log::m_logStream;
QMap<QString, EventID> Log::m_events;

QMap<QString, quint64> Log::m_eventCount;

void Log::init(QString logFilePath, QMap<QString, EventID> events)
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

    // TODO: replace pointer to object with setDevice()
    m_logStream = new QDataStream(m_log);

    m_events = events;
    
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
}

void Log::uninit()
{
    m_log->close();
    
    // TODO: delete pointers
}

void Log::write(Event* event)
{
    qDebug() << "write event in log" << m_events[event->name] << event->name;
    // qDebug() << "write std data" << event;
    (*m_logStream) << event->time << m_events[event->name];
    // (*m_logStream) << event->time;
    // qDebug() << "write event data" << event;
    // FIXME: uncomment it
    // event->record(*m_logStream);

    m_eventCount[event->name] += 1;
}
