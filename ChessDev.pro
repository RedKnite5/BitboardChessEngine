TEMPLATE = subdirs
SUBDIRS = lib app test UCI_app

#CONFIG += debug
DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00

app.depends = lib
test.depends = lib
UCI_app.depends = lib

