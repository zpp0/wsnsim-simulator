/**
 *
 * File: logtypes.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "logtypes.h"

QDataStream &operator<<(QDataStream &out, const lByteArray &bytearray)
{
    out.writeRawData(bytearray.array.constData(), bytearray.length);
    return out;
}
QDataStream &operator<<(QDataStream &out, const lString &string)
{
    out.writeRawData(string.string.constData(), string.length);
    return out;
}
