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

#include "loggableEvent.h"

/**
 *
 * класс предназначен для записи файла логов
 *
 **/

class log
{
public:
    
    static void init(QString logFilePath);
    static void uninit();

    static void writeLog(loggableEvent* event);
    
private:
    log();
    static QHash<QString, EventID> events;
    static QFile* m_log;
    static QDataStream* m_logStream;
};

#endif // LOG_H
