/**
 *
 * File: logtypes.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef LOGTYPES_H
#define LOGTYPES_H

#include <QtCore>

typedef quint64 lTime;
typedef int lInt32;
typedef quint8 lUint8;
typedef quint16 lUint16;
typedef quint32 lUint32;
typedef double lDouble;

struct lByteArray
{
lByteArray(QString bytearray) :length(bytearray.size()), array(bytearray.toAscii()) {}
    lUint8 length;
    QByteArray array;
};

struct lString
{
lString(QString string) :length(string.size()), string(string.toUtf8()) {}
    lUint16 length;
    QByteArray string;
};

QDataStream &operator<<(QDataStream &out, const lByteArray &event);
QDataStream &operator<<(QDataStream &out, const lString &event);

#endif // LOGTYPES_H
