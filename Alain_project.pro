#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T14:13:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

TARGET = Alain_project
TEMPLATE = app


SOURCES += main.cpp\
    configurator.cpp \
    logger.cpp \
    loggercommandadapter.cpp \
    logstrategysimple.cpp \
    mainwindow.cpp \
    plotter.cpp \
    qcustomplot.cpp \
    serial.cpp \
    serialcommandadapter.cpp

HEADERS  += mainwindow.h \
    configurationunit.h \
    configurator.h \
    datacommand.h \
    logger.h \
    loggercommandadapter.h \
    logstrategysimple.h \
    plotter.h \
    qcustomplot.h \
    serial.h \
    serialcommandadapter.h

FORMS    += mainwindow.ui
QT  += serialport

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
