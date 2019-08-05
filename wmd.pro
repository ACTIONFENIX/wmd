TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug

SOURCES += \
        downloader.cpp \
        main.cpp \
        readmanga.cpp \
        sitebase.cpp

QMAKE_CXXFLAGS += -Wall -Wextra

LIBS += -lcurl

HEADERS += \
    downloader.h \
    readmanga.h \
    sitebase.h
