TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -static
LIBS += -lIphlpapi
LIBS += -lws2_32

SOURCES += \
        App/Config/Loader.cpp \
        App/Config/Map.cpp \
        App/Config/MouseSynthLeftClick.cpp \
        App/Config/NetworkPinger.cpp \
        App/Config/PrinterHardwareInterface.cpp \
        App/FatalException.cpp \
        App/MouseSynth/LeftClick.cpp \
        App/Network/Pinger.cpp \
        App/PhoenixDown.cpp \
        App/Printer/HardwareInterface.cpp \
        App/Printer/Logic.cpp \
        App/Screen/Console.cpp \
        App/Screen/Interface.cpp \
        Libs/Exception/Exception.cpp \
        Libs/ICMP/File.cpp \
        Libs/ICMP/IPv4.cpp \
        Libs/ICMP/Ping.cpp \
        Libs/Serial/ComPort.cpp \
        Libs/Serial/Protocol.cpp

HEADERS += \
    App/Config/GreaterThenZeroValidator.h \
    App/Config/Loader.h \
    App/Config/Map.h \
    App/Config/MouseSynthLeftClick.h \
    App/Config/NetworkPinger.h \
    App/Config/PrinterHardwareInterface.h \
    App/FatalException.h \
    App/MouseSynth/LeftClick.h \
    App/Network/PingStatus.h \
    App/Network/Pinger.h \
    App/Printer/HardwareInterface.h \
    App/Printer/HardwareStatus.h \
    App/Printer/Logic.h \
    App/Printer/LogicStatus.h \
    App/Screen/Color.h \
    App/Screen/Console.h \
    App/Screen/Interface.h \
    Libs/Exception/Exception.h \
    Libs/ICMP/File.h \
    Libs/ICMP/IPv4.h \
    Libs/ICMP/Ping.h \
    Libs/ICMP/PingResponse.h \
    Libs/Serial/ComPort.h \
    Libs/Serial/Protocol.h \
    Libs/Util/CappedCounter.h \
    Libs/Util/Narrow.h \
    Libs/Util/Point.h \
    Libs/Util/Rectangle.h \
    Libs/Util/UniqueResource.h
