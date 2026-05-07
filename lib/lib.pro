TEMPLATE = lib
CONFIG += staticlib
TARGET = chess

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++20

QMAKE_CXXFLAGS += -fconstexpr-ops-limit=100000000  # 100 million
#QMAKE_CXXFLAGS += -g

QMAKE_CXXFLAGS += -std=c++20
QMAKE_CXXFLAGS_RELEASE += -flto -fuse-linker-plugin -ffat-lto-objects
QMAKE_CFLAGS_RELEASE   += -flto -fuse-linker-plugin -ffat-lto-objects
QMAKE_LFLAGS_RELEASE   += -O2 -flto -fuse-linker-plugin

RESOURCES += resources.qrc

SOURCES += \
    mainwindow.cpp \
    draggable.cpp \
    board.cpp \
    search.cpp \
    engine_worker.cpp 

HEADERS += \
    mainwindow.h \
    draggable.h \
    board.h \
    search.h \
    engine_worker.h 

FORMS += \
    mainwindow.ui



