######################################################################
# Automatically generated by qmake (2.01a) Thu Mar 8 16:57:19 2012
######################################################################

TEMPLATE = app
TARGET = simulator
DESTDIR = ../../bin/
OBJECTS_DIR = ./debug
DEPENDPATH += . include
INCLUDEPATH += . include ../../projectData ../interfaces/environment/ ../interfaces/hardware/ ../modules/radioChannel/ ../modules/RTX/ ../modules/timer/ ../modules/scene/
# CONFIG += static
CONFIG += release warn_off

DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

# Input
HEADERS += env.h \
           eventQueue.h \
           ../../projectData/projectParams.h \
           log.h \
           node.h \
           simulator.h \
           include/event.h \
           include/IEnvironment.h \
           include/IHardware.h \
           include/IModule.h \
           include/ISimulator.h \
           include/ISoftware.h \
           include/nodeEvent.h \
           include/types.h \
           ../interfaces/environment/IRadioChannel.h \
           ../interfaces/environment/IScene.h \ 
           ../interfaces/hardware/Irtx.h \ 
           ../interfaces/hardware/ITimer.h \
           ../modules/radioChannel/radioChannel.h \
           ../modules/scene/scene.h \
           ../modules/timer/timer.h \
           ../modules/RTX/rtx.h
SOURCES += eventQueue.cpp log.cpp main.cpp simulator.cpp event.cpp ../modules/RTX/rtx.cpp ../modules/radioChannel/radioChannel.cpp ../modules/scene/scene.cpp ../modules/timer/timer.cpp env.cpp node.cpp
