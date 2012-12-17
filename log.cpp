/**
 *
 * File: log.cpp
 * Description: Simulator binary log class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QDataStream>

#include "log.h"

QDataStream Log::m_logStream;
QString Log::m_errorString;

int Log::init(QString directory, QString logFileName)
{
    if (logFileName == "") {
        m_errorString = "Error: empty log file path";
        return 0;
    }

    QString logFilePath = directory + "/" + logFileName;

    // remove old log file with the same name
    QFile* m_log = new QFile(logFilePath);
    m_log->remove();

    if (!m_log->open(QIODevice::Append)) {
        m_errorString = "Error: cannot open log file " + logFilePath + "\n" + m_log->errorString();
        return 0;
    }

    m_logStream.setDevice(m_log);

    return 1;
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
            m_logStream << param.value.u8;
            break;
        case UINT16_TYPE:
            m_logStream << param.value.u16;
            break;
        case UINT32_TYPE:
            m_logStream << param.value.u32;
            break;
        case UINT64_TYPE:
            m_logStream << param.value.u64;
            break;
        case INT32_TYPE:
            m_logStream << param.value.i32;
            break;
        case BOOL_TYPE:
            m_logStream << (quint8)param.value.b;
            break;
        case DOUBLE_TYPE:
            m_logStream << param.value.d;
            break;
        case BYTE_ARRAY_TYPE:
        {
            m_logStream << param.value.byteArray.size;
            m_logStream.writeRawData(param.value.byteArray.data, param.value.byteArray.size);
            break;
        }
        case STRING_TYPE:
        {
            m_logStream << param.value.string.length;
            m_logStream.writeRawData(param.value.string.data, param.value.string.length);
            break;
        }
    case UNKNOWN_TYPE:
        break;
        }
    }
}

QString Log::errorString()
{
    return m_errorString;
}
