#-------------------------------------------------
#
# Project created by QtCreator 2018-08-06T11:37:02
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $$PWD/bin
TARGET = MsLanguageTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    codeeditor.cpp \
    LanguageWidget.cpp \
    ExportUntranslated.cpp \
    ExportNew.cpp \
    About.cpp \
    DeveloperMode.cpp \
    DifferentWidget.cpp \
    checkerror.cpp \
    message.cpp

HEADERS += \
        MainWindow.h \
    codeeditor.h \
    LanguageWidget.h \
    ExportUntranslated.h \
    ExportNew.h \
    About.h \
    DeveloperMode.h \
    DifferentWidget.h \
    checkerror.h \
    message.h

FORMS += \
        MainWindow.ui \
    LanguageWidget.ui \
    ExportUntranslated.ui \
    ExportNew.ui \
    About.ui \
    DeveloperMode.ui \
    DifferentWidget.ui \
    checkerror.ui \
    message.ui

RC_FILE += icon.rc

RESOURCES += \
    MsLanguageTool.qrc
