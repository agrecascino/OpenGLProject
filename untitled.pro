TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += main.cpp
QMAKE_CXXFLAGS += -std=c++0x
include(deployment.pri)
qtcAddDeployment()

unix|win32: LIBS += -lGL

unix|win32: LIBS += -lGLU

unix|win32: LIBS += -lGLEW

unix|win32: LIBS += -lglfw

HEADERS += \
    moduleplayer.h \
    entity.h \
    itsnotok.h








unix|win32: LIBS += -lportaudio

unix|win32: LIBS += -lpthread
unix|win32: LIBS += -lopenmpt







unix|win32: LIBS += -lNewton
