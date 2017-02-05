#-------------------------------------------------
#
# Project created by QtCreator 2017-01-16T18:58:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = map_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    map.cpp \
    utility.cpp \
    insertbox.cpp

HEADERS  += mainwindow.h \
    map.h \
    utility.h \
    insertbox.h
