TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

# Google Benchmark
INCLUDEPATH += /home/you/dev/benchmark/include

# Link against Google Benchmark and pthread
LIBS += -lbenchmark -lpthread

# Add library search path
LIBS += -L/home/you/dev/benchmark/build/src

LIBS += -L../lib -lchess -lgtest -lgtest_main
INCLUDEPATH += ../lib


SOURCES += main.cpp

DESTDIR = bin
TARGET = ChessTest

QMAKE_CXXFLAGS += -fconstexpr-ops-limit=100000000 -g  # 100 million
QMAKE_CXXFLAGS += -std=c++20

