QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -fconstexpr-ops-limit=100000000   # 100 million



# Add Google Benchmark include directory
INCLUDEPATH += /home/you/dev/benchmark/include

# Add library search path
LIBS += -L/home/you/dev/benchmark/build/src

# Link against Google Benchmark and pthread
LIBS += -lbenchmark -lpthread

# If using BENCHMARK_MAIN(), you may also want:
# LIBS += -lbenchmark_main

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    draggable.cpp \
    board.cpp

HEADERS += \
    mainwindow.h \
    draggable.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
