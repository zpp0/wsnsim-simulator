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

#include "event.h"

class EventHandler
{
public:
    EventHandler()
        : object_ptr(0)
        , stub_ptr(0)
        {}

    template <class T, void (T::*TMethod)(Event*)>
    static EventHandler from_method(T* object_ptr) {
        EventHandler d;
        d.object_ptr = object_ptr;
        d.stub_ptr = &method_stub<T, TMethod>;
        return d;
    }

    void operator()(Event* event) const {
        return (*stub_ptr)(object_ptr, event);
    }

private:
    typedef void (*stub_type)(void* object_ptr, Event*);

    void* object_ptr;
    stub_type stub_ptr;

    template <class T, void (T::*TMethod)(int)>
    static void method_stub(void* object_ptr, Event* event) {
        T* p = static_cast<T*>(object_ptr);
        return (p->*TMethod)(event);
    }
};

#endif // EVENTHANDLER_H
