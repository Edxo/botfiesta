#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T19:42:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Botfiesta
TEMPLATE = app


SOURCES += main.cpp\
    socket/socket.cpp \
    service/service.cpp \
    ui/mainwindow.cpp

HEADERS  += \
    socket/socket.h \
    service/service.h \
    ui/mainwindow.h

FORMS    += mainwindow.ui
