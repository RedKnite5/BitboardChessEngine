TEMPLATE = app
QT += core gui widgets

#CONFIG += debug
LIBS += -L../lib -lchess
INCLUDEPATH += ../lib

#QMAKE_CXXFLAGS += -g

PRE_TARGETDEPS += ../lib/libchess.a

SOURCES += main.cpp
