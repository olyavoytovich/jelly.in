QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jellyin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        game_controller.cpp \
        game_object.cpp \
        main.cpp \
        view.cpp

HEADERS += \
    abstract_game_controller.h \
    abstract_view.h \
    game_controller.h \
    game_object.h \
    view.h



