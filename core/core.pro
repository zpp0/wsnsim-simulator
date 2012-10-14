include(../simulator.pri)

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../../libs/projectData
INCLUDEPATH += . /usr/include/lua5.1
LIBS += -llua

CONFIG += console

# Input
HEADERS += env.h event.h eventQueue.h log.h node.h simulator.h luaHost.h module.h types.h project.h moduleAdapter.h moduleAdapter_lua.h eventhandler.hpp
SOURCES += env.cpp \
           event.cpp \
           eventQueue.cpp \
           log.cpp \
           node.cpp \
           simulator.cpp \
           moduleAdapter.cpp \
           moduleAdapter_lua.cpp \
           luaHost.cpp \
           project.cpp \
           main.cpp
