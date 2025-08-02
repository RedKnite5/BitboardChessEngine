TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += test/main_test.cpp

LIBS += -lgtest -lgtest_main -lpthread

DESTDIR = bin
TARGET = ChessTest

QMAKE_CXXFLAGS += -fconstexpr-ops-limit=100000000   # 100 million
