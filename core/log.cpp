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

int Log::init(QString logFilePath)
{
    if (logFilePath == "") {
        m_errorString = "Error: empty log file path";
        return 0;
    }

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
        QVariant value = param.value;
        switch(param.type) {
        case UINT8_TYPE:
            m_logStream << (quint8)value.toUInt();
            break;
        case UINT16_TYPE:
            m_logStream << (quint16)value.toUInt();
            break;
        case UINT32_TYPE:
            m_logStream << (quint32)value.toUInt();
            break;
        case UINT64_TYPE:
            m_logStream << (quint64)value.toUInt();
            break;
        case INT32_TYPE:
            m_logStream << (qint32)value.toInt();
            break;
        case BOOL_TYPE:
            m_logStream << (quint8)value.toInt();
            break;
        case DOUBLE_TYPE:
            m_logStream << value.toDouble();
            break;
        case BYTE_ARRAY_TYPE:
        {
            QByteArray array = value.toByteArray();
            m_logStream << (quint8)array.size();
            m_logStream.writeRawData(array.constData(), array.size());
            break;
        }
        case STRING_TYPE:
        {
            QString string = value.toString();
            m_logStream << (quint16)string.size();
            m_logStream.writeRawData(string.toUtf8().constData(), string.size());
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
