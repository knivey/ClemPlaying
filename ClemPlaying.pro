QT += core
QT += dbus
QT -= gui

QMAKE_CXXFLAGS += -std=c++14

TARGET = ClemPlaying
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

