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
    parsemanager.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    constants.h \
    parsemanager.h

FORMS    += mainwindow.ui

DESTDIR = $$PWD

INCLUDEPATH += /Users/Kirill/Downloads/ChartDirector/include

DEFINES += CHARTDIR_HIDE_OBSOLETE

LIBS += -L /Users/Kirill/Downloads/ChartDirector/lib -lchartdir

QMAKE_POST_LINK += cp /Users/Kirill/Downloads/ChartDirector/lib/libchartdir.5.dylib \"$(DESTDIR)\"

CONFIG -= app_bundle
CONFIG += c++11
