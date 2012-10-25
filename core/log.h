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

class Log
{
public:
    static void init(QString logFilePath);
    static void uninit();

    static void write(Event* event);

private:
    static QDataStream m_logStream;
};

#endif // LOG_H
