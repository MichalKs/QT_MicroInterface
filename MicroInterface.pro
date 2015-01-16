#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T22:48:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MicroInterface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc
