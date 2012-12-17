include(../wsnsim.pri)

QT  += core xml

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../libs/projectData

win32 {
    INCLUDEPATH += ../libs/luajit/src
    LIBS += ../../bin/libluajit.a
    LIBS += ../../bin/libprojectData.a
}

unix {
    INCLUDEPATH += . /usr/include/luajit-2.0
    LIBS += -lluajit-5.1
    LIBS += -L../../bin -lprojectData
}

CONFIG += console

# Input
HEADERS += event.h eventQueue.h log.h simulator.h luaHost.h module.h virtualTime.h project.h IHandler.h eventHandler.h eventNodeHandler.h systemEventHandler.h
SOURCES += eventQueue.cpp \
           log.cpp \
           simulator.cpp \
           module.cpp \
           event.cpp \
           luaHost.cpp \
           project.cpp \
           systemEventHandler.cpp \
           eventHandler.cpp \
           eventNodeHandler.cpp \
           IHandler.cpp \
           main.cpp
