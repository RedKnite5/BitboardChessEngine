TEMPLATE = subdirs
SUBDIRS = lib app test

DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00

app.depends = lib
test.depends = lib

