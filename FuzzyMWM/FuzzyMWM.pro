#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T21:36:16
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FuzzyMWM
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    fuzzyinferenceengine.cpp \
    rulebase.cpp \
    dataset.cpp \
    fuzzyrulebasecreator.cpp \
    fuzzysetcontainer.cpp \
    triangularfuzzyset.cpp

HEADERS += \
    fuzzyset.h \
    fuzzyinferenceengine.h \
    rulebase.h \
    dataset.h \
    fuzzyrulebasecreator.h \
    fuzzysetcontainer.h \
    triangularfuzzyset.h
