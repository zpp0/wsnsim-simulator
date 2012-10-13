/**
 *
 * File: log.cpp
 * Description: Simulator binary log class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QDataStream>
#include <iostream>

#include "log.h"

QFile* Log::m_log;
QDataStream* Log::m_logStream;
QMap<QString, EventID> Log::m_events;
QMap<QString, QList<LogDataType> > Log::m_eventArgTypes;

QMap<QString, quint64> Log::m_eventCount;

void Log::init(QString logFilePath,
               QMap<QString, EventID> events,
               QMap<QString, QList<QString> > eventArgTypes)
{
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

    foreach (QString event, eventArgTypes.keys()) {
        qDebug() << event;

        foreach (QString argType, eventArgTypes[event]) {

            LogDataType type = UNKNOWN_TYPE;
            if (argType == "uint8")          type = UINT8_TYPE;
            else if (argType == "uint16")    type = UINT16_TYPE;
            else if (argType == "uint32")    type = UINT32_TYPE;
            else if (argType == "uint64")    type = UINT64_TYPE;
            else if (argType == "int32")     type = INT32_TYPE;
            else if (argType == "bool")      type = BOOL_TYPE;
            else if (argType == "double")    type = DOUBLE_TYPE;
            else if (argType == "ByteArray") type = BYTE_ARRAY_TYPE;
            else if (argType == "string")    type = STRING_TYPE;

            qDebug() << argType << type;

            m_eventArgTypes[event] += type;
        }
    }
}

void Log::uninit()
{
    m_log->close();

    // TODO: delete pointers
}

void Log::write(Event* event)
{
    qDebug() << "write event in log" << m_events[event->name] << event->name;

    (*m_logStream) << event->time << m_events[event->name];

    qDebug() << event->params.size();

    for (int i = 0; i < event->params.size(); i++) {
        switch(m_eventArgTypes[event->name][i]) {
        case UINT8_TYPE:
            (*m_logStream) << (quint8)event->params[i].toUInt();
            break;
        case UINT16_TYPE:
            (*m_logStream) << (quint16)event->params[i].toUInt();
            break;
        case UINT32_TYPE:
            (*m_logStream) << (quint32)event->params[i].toUInt();
            break;
        case UINT64_TYPE:
            (*m_logStream) << (quint64)event->params[i].toUInt();
            break;
        case INT32_TYPE:
            (*m_logStream) << (qint32)event->params[i].toInt();
            break;
        case BOOL_TYPE:
            (*m_logStream) << event->params[i].toInt();
            break;
        case DOUBLE_TYPE:
            (*m_logStream) << event->params[i].toDouble();
            break;
        case BYTE_ARRAY_TYPE:
        {
            QByteArray array = event->params[i].toByteArray();
            (*m_logStream) << (quint8)array.size();
            (*m_logStream).writeRawData(array.constData(), array.size());
            break;
        }
        case STRING_TYPE:
        {
            QString string = event->params[i].toString();
            (*m_logStream) << (quint16)string.size();
            (*m_logStream).writeRawData(string.toUtf8().constData(), string.size());
            break;
        }
        }
    }
}
