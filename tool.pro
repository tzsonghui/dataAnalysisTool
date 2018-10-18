#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T20:25:06
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    unpack.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    unpack.h \
    config.h

FORMS    += mainwindow.ui

RESOURCES += \
    sources.qrc


OTHER_FILES += tool.rc

RC_FILE += tool.rc
