QT       += core gui
ICON      = ./icon.png

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++17
CONFIG += lrelease
CONFIG += embed_translations

QMAKE_CC = clang
QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++17

SOURCES += $$files(../*.cc, true)
HEADERS += $$files(../*.h, true)
INCLUDEPATH += $$dirname(HEADERS)
