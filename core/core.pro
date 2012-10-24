include(../simulator.pri)

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../../libs/projectData
INCLUDEPATH += . /usr/include/lua5.1
LIBS += -llua

CONFIG += console

# Input
HEADERS += event.h eventQueue.h log.h simulator.h luaHost.h module.h types.h project.h moduleAdapter.h moduleAdapter_lua.h eventhandler.hpp simulator_api.h luaEventHandler.h
SOURCES += eventQueue.cpp \
           log.cpp \
           simulator.cpp \
           moduleAdapter.cpp \
           moduleAdapter_lua.cpp \
           luaHost.cpp \
           project.cpp \
           simulator_api.cpp \
           luaEventHandler.cpp \
           main.cpp
