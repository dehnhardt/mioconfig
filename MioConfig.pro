#-------------------------------------------------
#
# Project created by QtCreator 2017-03-10T21:28:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MioConfig
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += __LINUX_ALSA__
DEFINES += __MIO_SIMULATE__

DEFINES += __RTMIDI_DEBUG__
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=\
        src/RtMidi.cpp \
    src/main.cpp \
    src/miomain.cpp \
    src/device.cpp \
    src/sysex/midi.cpp \
    src/devicedetection.cpp \
    src/config/configuration.cpp \
    src/devicedetectionprocessor.cpp \
    src/sysex/sysexmessage.cpp \
    src/sysex/devicedetectionquery.cpp \
    src/deviceselectiontablemodel.cpp \
    src/sysex/commands.cpp \
    src/sysex/infos.cpp \
    src/sysex/deviceinfo.cpp

INCLUDEPATH += /usr/include/libusb-1.0/

HEADERS  += src/miomain.h \
    src/RtMidi.h \
    src/device.h \
    src/sysex/midi.h \
    src/devicedetection.h \
    src/config/configuration.h \
    src/devicedetectionprocessor.h \
    src/sysex/sysexmessage.h \
    src/sysex/devicedetectionquery.h \
    src/deviceselectiontablemodel.h \
    src/sysex/commands.h \
    src/sysex/infos.h \
    src/sysex/deviceinfo.h

FORMS    += src/miomain.ui \
    src/devicedetection.ui

unix:!macx: LIBS += -lasound \
                    -lpthread \
                    -lusb-1.0
