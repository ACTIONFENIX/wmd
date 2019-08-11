TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug

SOURCES += \
        main.cpp \
        mangabase.cpp \
        mangaexception.cpp \
        mangafactory.cpp \
        readmanga.cpp

QMAKE_CXXFLAGS += -Wall -Wextra

LIBS += -lcurl -lstdc++fs

HEADERS += \
    mangabase.h \
    mangaexception.h \
    mangafactory.h \
    readmanga.h
