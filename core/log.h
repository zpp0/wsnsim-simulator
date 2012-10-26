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
#include <QUdpSocket>

#include "event.h"

class Log
{
public:
    static int init(QString logFilePath);
    static void uninit();

    static void write(Event* event);

    static QString errorString();

private:
    static QDataStream m_logStream;
    static QUdpSocket m_socket;
    static QHostAddress m_addr;
    static quint16 m_port;
    static QString m_errorString;
};

#endif // LOG_H
