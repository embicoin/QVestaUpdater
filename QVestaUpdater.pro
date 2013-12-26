#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T18:23:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVestaUpdater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QVestaUpdaterTrayIcon.cpp \
    QVestaUpdater.cpp

HEADERS  += mainwindow.h \
    QVestaUpdaterTrayIcon.h \
    QVestaUpdater.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=
