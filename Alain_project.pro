#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T14:13:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Alain_project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    serial.h

FORMS    += mainwindow.ui
QT  += serialport