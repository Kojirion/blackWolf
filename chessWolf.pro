TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    controller/boardmaster.cpp \
    views/components/piecesprite.cpp \
    position.cpp \
    guimanager.cpp \
    boardmove.cpp \
    completemove.cpp \
    chessengine.cpp \
    views/components/piecesbimap.cpp \
    ext/file_descriptor.cpp \
    resourcemanager.cpp \
    msmmanager.cpp \
    controller/components/settingsstate.cpp \
    boardstate.cpp \
    views/moveview.cpp \
    views/boardcanvas.cpp \
    views/clocksview.cpp \
    model/gamedata.cpp \
    blackWolf.cpp \
    views/statusview.cpp \
    controller/components/buttonbox.cpp \
    controller/components/sidechoicewindow.cpp

QMAKE_CXXFLAGS += -std=c++11

LIBS += /usr/local/lib/libboost_system.so
LIBS += /usr/local/lib/libsfgui.so
LIBS += /usr/local/lib/libthor.so
LIBS += /usr/local/lib/libsfml-graphics.so.2
LIBS += /usr/local/lib/libsfml-window.so.2
LIBS += /usr/local/lib/libsfml-system.so.2
LIBS += -L/usr/local/lib -lGLU


HEADERS += \
    controller/boardmaster.h \
    views/components/piecesprite.h \
    position.h \
    guimanager.h \
    boardmove.h \
    completemove.h \
    chessengine.h \
    views/components/piecesbimap.h \
    blackWolf.h \
    resourcemanager.h \
    msmmanager.h \
    controller/components/settingsstate.h \
    boardstate.h \
    views/moveview.h \
    views/boardcanvas.h \
    views/clocksview.h \
    model/gamedata.h \
    views/statusview.h \
    controller/components/buttonbox.h \
    controller/components/sidechoicewindow.h

OTHER_FILES += \
    notes.txt \
    README.txt \
    licence.txt

