/**
 *
 * File: log.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QFile>

#include "event.h"

class Log
{
public:
    // TODO: remove second argument?
    static void init(QString logFilePath, QMap<QString, EventID> events);
    static void uninit();

    // TODO: return bool if event was successfully recorded
    static void write(Event* event);
    
private:
    static QMap<QString, EventID> m_events;
    static QFile* m_log;
    static QDataStream* m_logStream;

    // TODO: do it public
    static QMap<QString, quint64> m_eventCount;
};

#endif // LOG_H
