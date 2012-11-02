include(../wsnsim.pri)

QT  += core

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../libs/projectData
INCLUDEPATH += . /usr/include/luajit-2.0
LIBS += -lluajit-5.1

CONFIG += console

# Input
HEADERS += event.h eventQueue.h log.h simulator.h luaHost.h module.h types.h project.h eventhandler.h
SOURCES += eventQueue.cpp \
           log.cpp \
           simulator.cpp \
           module.cpp \
           luaHost.cpp \
           project.cpp \
           eventHandler.cpp \
           main.cpp
