#-------------------------------------------------
#
# Project created by QtCreator 2011-08-14T22:10:58
#
#-------------------------------------------------

QT       += testlib
QT       += gui widgets

TARGET = tst_renamercoretest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    dummymethod.cpp \
    tst_renamercoretest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    dummymethod.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RenamerCore/release/ -lRenamerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RenamerCore/debug/ -lRenamerCore
else:symbian: LIBS += -lRenamerCore
else:unix: LIBS += -L$$OUT_PWD/../RenamerCore/ -lRenamerCore

INCLUDEPATH += $$PWD/../RenamerCore
DEPENDPATH += $$PWD/../RenamerCore

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/release/libRenamerCore.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/debug/libRenamerCore.a
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/libRenamerCore.a
