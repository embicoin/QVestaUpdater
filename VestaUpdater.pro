#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T18:23:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VestaUpdater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    VestaUpdater.cpp \
    QVestaUpdaterTrayIcon.cpp

HEADERS  += mainwindow.h \
    VestaUpdater.h \
    QVestaUpdaterTrayIcon.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=
