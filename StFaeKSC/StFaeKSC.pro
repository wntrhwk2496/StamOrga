QT += core network
QT -= gui

CONFIG += c++11

TARGET = StFaeKSC
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../Common/General/backgroundcontroller.cpp \
    ../Common/General/backgroundworker.cpp \
    Network/udpserver.cpp \
    ../Common/Network/messagebuffer.cpp \
    ../Common/Network/messageprotocol.cpp \
    ../Common/Network/messagecommand.cpp \
    General/globaldata.cpp \
    General/listeduser.cpp \
    General/console.cpp \
    Network/udpdataserver.cpp \
    ../Common/General/logging.cpp \
    ../Common/General/globalfunctions.cpp \
    General/dataconnection.cpp

HEADERS += \
    ../Common/General/backgroundcontroller.h \
    ../Common/General/backgroundworker.h \
    ../Common/General/config.h \
    Network/udpserver.h \
    ../Common/Network/messagebuffer.h \
    ../Common/Network/messageprotocol.h \
    ../Common/General/globaltiming.h \
    ../Common/Network/messagecommand.h \
    General/globaldata.h \
    General/listeduser.h \
    ../Common/General/globalfunctions.h \
    General/console.h \
    General/usercommand.h \
    Network/udpdataserver.h \
    Network/connectiondata.h \
    ../Common/General/logging.h \
    General/dataconnection.h


unix {
#    QMAKE_POST_LINK = mkdir -p $$OUTPUT_FOLDER && cp $$TARGET $$OUTPUT_FOLDER
    QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"

    target.path = /home/pi/StFaeKSC
    INSTALLS += target

}
