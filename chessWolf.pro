TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    controller/boardmaster.cpp \
    views/components/piecesprite.cpp \
    common/position.cpp \
    guimanager.cpp \
    common/boardmove.cpp \
    common/completemove.cpp \
    chessengine.cpp \
    views/components/piecesbimap.cpp \
    ext/file_descriptor.cpp \
    resourcemanager.cpp \
    controller/components/settingsstate.cpp \
    views/moveview.cpp \
    views/boardcanvas.cpp \
    views/clocksview.cpp \
    model/gamedata.cpp \
    blackWolf.cpp \
    views/statusview.cpp \
    controller/components/buttonbox.cpp \
    controller/components/sidechoicewindow.cpp \
    views/components/firework.cpp

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
    common/position.h \
    guimanager.h \
    common/boardmove.h \
    common/completemove.h \
    chessengine.h \
    views/components/piecesbimap.h \
    blackWolf.h \
    resourcemanager.h \
    controller/components/settingsstate.h \
    views/moveview.h \
    views/boardcanvas.h \
    views/clocksview.h \
    model/gamedata.h \
    views/statusview.h \
    controller/components/buttonbox.h \
    controller/components/sidechoicewindow.h \
    options.h \
    views/components/firework.h

OTHER_FILES += \
    notes.txt \
    README.txt \
    licence.txt

