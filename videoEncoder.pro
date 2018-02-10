#-------------------------------------------------
#
# Project created by QtCreator 2017-12-28T21:13:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videoEncoder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS
LIBS += -LC:/ffmpeg/Build/Windows10/x64/bin -lavcodec -lavformat -lavutil -lswscale -lavdevice
INCLUDEPATH += "C:/ffmpeg/Build/Windows10/x64/include"
LIBS += -lkernel32 -luser32
SOURCES += \
        main.cpp \
        videoenncoder.cpp \
        videoinfo.cpp \
    ffmpegtest.cpp \
    videoinfowidget.cpp \
    videoprocess.cpp \
    videoinfo_thread.cpp

HEADERS += \
        videoenncoder.h \
        videoinfo.h \
    ffmpegtest.h \
    videoinfowidget.h \
    videoprocess.h \
    videoinfo_thread.h

FORMS += \
    videoenncoder.ui \
    videoinfowidget.ui

RESOURCES += \
    icons.qrc

