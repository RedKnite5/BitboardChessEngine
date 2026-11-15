TEMPLATE = app

CONFIG += debug
LIBS += -L../lib -lchess
INCLUDEPATH += ../lib

QMAKE_CXXFLAGS += -std=c++20

PRE_TARGETDEPS += ../lib/libchess.a

SOURCES += main.cpp
