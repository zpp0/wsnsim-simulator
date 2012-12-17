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

#include <QTextStream>

#include "event.h"

class Log
{
public:
    static int init(QString directory, QString logFileName);
    static void uninit();

    static void write(Event* event);

    static void writeError(QString message);

    static QString errorString();

private:
    static QDataStream m_logStream;
    static QTextStream m_errorStream;
    static QString m_errorString;
};

#endif // LOG_H
