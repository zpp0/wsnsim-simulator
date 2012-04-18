/**
 *
 * File: event.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "event.h"

quint64 event::count = 0;

event::event()
{
    count++;
    recordable = false;
}

