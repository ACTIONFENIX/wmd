TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug

SOURCES += \
        grouple.cpp \
        grouplebase.cpp \
        main.cpp \
        mangabase.cpp \
        mangaexception.cpp \
        mangafactory.cpp

QMAKE_CXXFLAGS += -Wall -Wextra

LIBS += -lcurl -lstdc++fs

HEADERS += \
    grouple.h \
    grouplebase.h \
    mangabase.h \
    mangaexception.h \
    mangafactory.h
