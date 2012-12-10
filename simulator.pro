include(../wsnsim.pri)

QT  += core

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../libs/projectData
INCLUDEPATH += . /usr/include/luajit-2.0
LIBS += -lluajit-5.1

CONFIG += console

# Input
HEADERS += event.h eventQueue.h log.h simulator.h luaHost.h module.h virtualTime.h project.h IHandler.h eventHandler.h eventNodeHandler.h
SOURCES += eventQueue.cpp \
           log.cpp \
           simulator.cpp \
           module.cpp \
           event.cpp \
           luaHost.cpp \
           project.cpp \
           eventHandler.cpp \
           eventNodeHandler.cpp \
           IHandler.cpp \
           main.cpp
