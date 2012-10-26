/**
 *
 * File: log.cpp
 * Description: Simulator binary log class
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QDataStream>
#include <QApplication>

#include "log.h"
#include "iclientrealtimesettings.h"

QDataStream Log::m_logStream;
QString Log::m_errorString;

QUdpSocket Log::m_socket;
QHostAddress Log::m_addr;
quint16 Log::m_port;

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

    QLibrary globalSettings("./globalSettings");
    typedef IClientRealTimeSettings*(*getClientRealTimeSettings) ();
    getClientRealTimeSettings func = (getClientRealTimeSettings) globalSettings.resolve("getClientRealTimeSettings");

    IClientRealTimeSettings* rtSettings = func();
    m_addr = QHostAddress(rtSettings->ip("simulator"));
    m_port = rtSettings->port("simulator");
    rtSettings->setProjectPath("simulator", QApplication::applicationDirPath() + "project.xml");

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
    QByteArray eventArr;
    QDataStream str(&eventArr, QIODevice::WriteOnly);
    str << event->time << event->ID;

    foreach(EventParam param, event->params) {
        switch(param.type) {
        case UINT8_TYPE:
            str << param.value.u8;
            break;
        case UINT16_TYPE:
            str << param.value.u16;
            break;
        case UINT32_TYPE:
            str << param.value.u32;
            break;
        case UINT64_TYPE:
            str << param.value.u64;
            break;
        case INT32_TYPE:
            str << param.value.i32;
            break;
        case BOOL_TYPE:
            str << param.value.b;
            break;
        case DOUBLE_TYPE:
            str << param.value.d;
            break;
        case BYTE_ARRAY_TYPE:
        {
            str << param.value.byteArray.size;
            str.writeRawData(param.value.byteArray.data, param.value.byteArray.size);
            break;
        }
        case STRING_TYPE:
        {
            str << param.value.string.length;
            str.writeRawData(param.value.string.data, param.value.string.length);
            break;
        }
    case UNKNOWN_TYPE:
        break;
        }
    }

    m_logStream.writeRawData(eventArr.constData(), eventArr.size());
    m_socket.writeDatagram(eventArr, m_addr, m_port);
}

QString Log::errorString()
{
    return m_errorString;
}
