#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T18:24:01
#
#-------------------------------------------------

QT       += core gui sql xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Unilever
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    parsemanager.cpp \
    qchartviewer.cpp \
    employeeview.cpp \
    itemcolordelegate.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    constants.h \
    parsemanager.h \
    qchartviewer.h \
    employeeview.h \
    itemcolordelegate.h

FORMS    += mainwindow.ui \
    employeecard.ui

DESTDIR = $$PWD

INCLUDEPATH += C:\projects\qt\chartdir_cpp_win32\ChartDirector\include

DEFINES += CHARTDIR_HIDE_OBSOLETE

LIBS += C:\projects\qt\chartdir_cpp_win32\ChartDirector\lib\chartdir51.lib

CONFIG -= app_bundle
CONFIG += c++11
