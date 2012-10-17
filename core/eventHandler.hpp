/**
 *
 * File: eventHandler.hpp
 * Description: Event Handler delegate
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QtCore>

#include "types.h"

class EventHandler
{
public:
    EventHandler()
        : object_ptr(0)
        , stub_ptr(0)
        {}

    template <class T, void (T::*TMethod)(EventID, QVariantList)>
    static EventHandler from_method(T* object_ptr) {
        EventHandler d;
        d.object_ptr = object_ptr;
        d.stub_ptr = &method_stub<T, TMethod>;
        return d;
    }

    void operator()(EventID event, QVariantList params) const {
        return (*stub_ptr)(object_ptr, event, params);
    }

private:
    typedef void (*stub_type)(void* object_ptr, EventID, QVariantList);

    void* object_ptr;
    stub_type stub_ptr;

    template <class T, void (T::*TMethod)(int)>
    static void method_stub(void* object_ptr, EventID eventID, QVariantList params) {
        T* p = static_cast<T*>(object_ptr);
        return (p->*TMethod)(eventID, params);
    }
};

#endif // EVENTHANDLER_H
