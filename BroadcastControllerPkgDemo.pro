#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T17:43:21
#
#-------------------------------------------------

QT       += core gui

TARGET = BroadcastControllerPkgDemo
TEMPLATE = app
include(BroadcastControllerPkg/BroadcastControllerPkg.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DEFINES += $$(ARCH)
contains( DEFINES, arm ) {

    mytarget.commands = scp ./${TARGET} root@192.168.1.243:/
}


mytarget.target = a

mytarget.depends = all



QMAKE_EXTRA_TARGETS += mytarget
