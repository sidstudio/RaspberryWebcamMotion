#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T16:00:24
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebcamTVClient
TEMPLATE = app

CONFIG += c++11

LIBS *= -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_objdetect

SOURCES += main.cpp\
        dialog.cpp \
    mainwindow.cpp \
    connection.cpp \
    pi2client.cpp

HEADERS  += dialog.h \
    mainwindow.h \
    connection.h \
    pi2client.h \
    type.h

FORMS    += dialog.ui \
    mainwindow.ui

RESOURCES += \
    resourse.qrc
