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




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5705/bin/release/ -lopenmpt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5705/bin/debug/ -lopenmpt
else:unix: LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5705/bin/ -lopenmpt

INCLUDEPATH += $$PWD/../Downloads/libopenmpt-0.2.5705/libopenmpt
DEPENDPATH += $$PWD/../Downloads/libopenmpt-0.2.5705/libopenmpt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5705/bin/release/libopenmpt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5705/bin/debug/libopenmpt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5705/bin/release/openmpt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5705/bin/debug/openmpt.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5705/bin/libopenmpt.a







unix|win32: LIBS += -lNewton
