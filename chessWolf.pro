TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    boardmaster.cpp \
    piecesprite.cpp \
    position.cpp \
    guimanager.cpp \
    boardmove.cpp \
    completemove.cpp \
    chessengine.cpp \
    piecesbimap.cpp \
    exec-stream.cpp

QMAKE_CXXFLAGS += -std=c++11 -pthread


LIBS += -pthread
LIBS += /usr/local/lib/libsfgui.so
LIBS += /usr/local/lib/libthor.so
LIBS += /usr/local/lib/libsfml-graphics.so.2
LIBS += /usr/local/lib/libsfml-window.so.2
LIBS += /usr/local/lib/libsfml-system.so.2
LIBS += -L/usr/local/lib -lGLU


HEADERS += \
    boardmaster.h \
    piecesprite.h \
    position.h \
    guimanager.h \
    boardmove.h \
    completemove.h \
    chessengine.h \
    piecesbimap.h

OTHER_FILES += \
    notes.txt \
    README.txt \
    licence.txt

