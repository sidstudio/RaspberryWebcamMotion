#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T12:37:32
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

CONFIG += c++11

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS *= -lopencv_core \
        -lopencv_video \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_objdetect \
        -lX11

SOURCES += main.cpp \
    pi2sslserver.cpp \
    clientconnection.cpp \
    videocapture.cpp \
    imageprocess.cpp \
    cconnectionspool.cpp \
    pooldetections.cpp \
    fileservice.cpp

HEADERS += \
    pi2sslserver.h \
    serverconfig.h \
    clientconnection.h \
    videocapture.h \
    type.h \
    imageprocess.h \
    cconnectionspool.h \
    pooldetections.h \
    fileservice.h
