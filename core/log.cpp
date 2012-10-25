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

QDataStream Log::m_logStream;

void Log::init(QString logFilePath)
{
    // TODO: return 0 and error string
    if (logFilePath == "") {
        std::cerr << "Error: cannot open log file " << logFilePath.toStdString() << std::endl;
        exit(1);
    }

    //для начала, удалим файл логов для того, чтобы создать новый
    // удаляем старый файл лога
    QFile* m_log = new QFile(logFilePath);
    m_log->remove();

    if (!m_log->open(QIODevice::Append)) {
        std::cerr << "Error: cannot open log file " << logFilePath.toStdString() << std::endl;
        exit(1);
    }

    // TODO: replace pointer to object with setDevice()
    m_logStream.setDevice(m_log);
}

void Log::uninit()
{
    QIODevice* log = m_logStream.device();
    log->close();
    delete log;
}

void Log::write(Event* event)
{
    m_logStream << event->time << event->ID;

    foreach(EventParam param, event->params) {
        switch(param.type) {
        case UINT8_TYPE:
            m_logStream << (quint8)param.value.toUInt();
            break;
        case UINT16_TYPE:
            m_logStream << (quint16)param.value.toUInt();
            break;
        case UINT32_TYPE:
            m_logStream << (quint32)param.value.toUInt();
            break;
        case UINT64_TYPE:
            m_logStream << (quint64)param.value.toUInt();
            break;
        case INT32_TYPE:
            m_logStream << (qint32)param.value.toInt();
            break;
        case BOOL_TYPE:
            m_logStream << (quint8)param.value.toInt();
            break;
        case DOUBLE_TYPE:
            m_logStream << param.value.toDouble();
            break;
        case BYTE_ARRAY_TYPE:
        {
            QByteArray array = param.value.toByteArray();
            m_logStream << (quint8)array.size();
            m_logStream.writeRawData(array.constData(), array.size());
            break;
        }
        case STRING_TYPE:
        {
            QString string = param.value.toString();
            m_logStream << (quint16)string.size();
            m_logStream.writeRawData(string.toUtf8().constData(), string.size());
            break;
        }
        }
    }
}
