INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
include(qextserialport/src/qextserialport.pri)
HEADERS += \
    BroadcastControllerPkg/callstateparser.h \
    BroadcastControllerPkg/trainstateparser.h \
    BroadcastControllerPkg/railwaystateparser.h \
    BroadcastControllerPkg/broadcastmanager.h \
    BroadcastControllerPkg/basicparser.h

SOURCES += \
    BroadcastControllerPkg/callstateparser.cpp \
    BroadcastControllerPkg/trainstateparser.cpp \
    BroadcastControllerPkg/railwaystateparser.cpp \
    BroadcastControllerPkg/broadcastmanager.cpp
