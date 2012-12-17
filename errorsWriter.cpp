/**
 *
 * File: errorsWriter.cpp
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#include "errorsWriter.h"

#include <QTextStream>

QFile* ErrorsWriter::m_file;

int ErrorsWriter::init(QString directory, QString errorsFileName)
{
    QString errorsFilePath = directory + "/" + errorsFileName;

    // remove old log file with the same name
    m_file = new QFile(errorsFilePath);
    m_file->remove();

    return 1;
}

void ErrorsWriter::uninit()
{
    m_file->close();
    delete m_file;
}

void ErrorsWriter::write(QString message)
{
    m_file->open(QIODevice::Append);
    QTextStream(m_file) << message << "\n";
}
