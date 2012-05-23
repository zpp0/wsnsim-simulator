include(../simulator.pri)

TEMPLATE = app
TARGET = simulator
INCLUDEPATH += . ../../libs/projectData

# Input
HEADERS += env.h event.h eventQueue.h log.h node.h simulator.h
SOURCES += env.cpp \
           event.cpp \
           eventQueue.cpp \
           log.cpp \
           main.cpp \
           node.cpp \
           simulator.cpp
