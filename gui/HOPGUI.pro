#-------------------------------------------------
#
# Project created by QtCreator 2014-08-17T00:14:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DESTDIR = ../bin

release:OBJECTS_DIR = ../build/release
release:MOC_DIR = ../build/release
release:UI_DIR = ../build/release

debug:OBJECTS_DIR = ../build/debug
debug:MOC_DIR = ../build/debug
debug:UI_DIR = ../build/debug

SOURCES += main.cpp\
        mainWindow.cpp

HEADERS  += mainWindow.hpp

FORMS    += mainWindow.ui
