#-------------------------------------------------
#
# Project created by QtCreator 2011-08-14T22:10:21
#
#-------------------------------------------------

TARGET = RenamerCore
TEMPLATE = lib
CONFIG += staticlib

SOURCES += renamercore.cpp \
    renamemethod.cpp \
    renameassistant.cpp \
    regexpmethod.cpp \
    queryassistant.cpp \
    query.cpp \
    rename.cpp \
    renamecommand.cpp \
    sequencemethod.cpp \
    parentfoldermethod.cpp

HEADERS += renamercore.h \
    renamemethod.h \
    renameassistant.h \
    regexpmethod.h \
    queryassistant.h \
    query.h \
    rename.h \
    renamecommand.h \
    sequencemethod.h \
    parentfoldermethod.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
