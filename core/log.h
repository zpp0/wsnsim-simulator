/**
 *
 * File: log.h
 * Description: Simulator binary log class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QFile>

#include "event.h"
#include "logDataTypes.h"

class Log
{
public:
    static void init(QString logFilePath,
                     QMap<QString, EventID> events,
                     QMap<QString, QList<QString> > eventArgTypes);
    static void uninit();

    static void write(Event* event);

private:
    static QMap<QString, EventID> m_events;
    static QMap<QString, QList<LogDataType> > m_eventArgTypes;
    static QFile* m_log;
    static QDataStream* m_logStream;

    // TODO: do it public
    static QMap<QString, quint64> m_eventCount;
};

#endif // LOG_H
