#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T17:00:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = gpi
CONFIG(debug, debug|release) {
    TEMPLATE = app
    SOURCES += main.cpp
} else {
    TEMPLATE = lib
    DEFINES += GPI_LIBRARY

    unix {
        target.path = /usr/lib
        INSTALLS += target
    }
}

SOURCES += \
    chromeosome.cpp \
    meprogram.cpp \
    gpi.cpp \
    fpspopulation.cpp

HEADERS += \
    chromosome.h \
    gpi.h \
    meprogram.h \
    fpspopulation.h \
    individual.h

CONFIG += c++11
