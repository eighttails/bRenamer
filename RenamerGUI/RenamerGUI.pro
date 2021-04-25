#-------------------------------------------------
#
# Project created by QtCreator 2011-09-09T21:37:11
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = RenamerGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        querylineedit.cpp \
        renamelineedit.cpp

HEADERS  += mainwindow.h \
    querylineedit.h \
    renamelineedit.h

FORMS    += mainwindow.ui







win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../RenamerCore/release/ -lRenamerCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../RenamerCore/debug/ -lRenamerCore
else:symbian: LIBS += -lRenamerCore
else:unix: LIBS += -L$$OUT_PWD/../RenamerCore/ -lRenamerCore

INCLUDEPATH += $$PWD/../RenamerCore
DEPENDPATH += $$PWD/../RenamerCore

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/release/libRenamerCore.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/debug/libRenamerCore.a
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../RenamerCore/libRenamerCore.a
