include(../simulator.pri)

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../../libs/projectData
INCLUDEPATH += . /usr/include/lua5.1
LIBS += -llua

CONFIG += console

# Input
HEADERS += env.h event.h eventQueue.h log.h node.h simulator.h luaHost.h modules.h types.h project.h moduleAdapter_lua.h
SOURCES += env.cpp \
           event.cpp \
           eventQueue.cpp \
           log.cpp \
           node.cpp \
           simulator.cpp \
           moduleAdapter_lua.cpp \
           luaHost.cpp \
           project.cpp \
           main.cpp
