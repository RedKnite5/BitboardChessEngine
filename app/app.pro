TEMPLATE = app
QT += core gui widgets

LIBS += -L../lib -lchess
INCLUDEPATH += ../lib

PRE_TARGETDEPS += ../lib/libchess.a

SOURCES += main.cpp
