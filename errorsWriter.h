/**
 *
 * File: errorsWriter.h
 * Author: Alexander Yarygin <yarygin.alexander@gmail.com>
 *
 **/

#ifndef ERRORSWRITER_H
#define ERRORSWRITER_H

#include <QString>
#include <QFile>

class ErrorsWriter
{
public:
    static int init(QString directory, QString errorFileName);
    static void uninit();

    static void write(QString message);

private:
    static QFile* m_file;
};

#endif // ERRORSWRITER_H
