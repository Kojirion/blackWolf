cmake_minimum_required(VERSION 2.8)

#project name
project(blackWolf)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

#find_package( SFML 2 REQUIRED COMPONENTS GRAPHICS WINDOW SYSTEM )
#link_directories(/usr/local/lib/)

#directories
#add_subdirectory(common)
#add_subdirectory(model)
#add_subdirectory(views)
#add_subdirectory(controller)
#add_subdirectory(ext)

set(bw_HDRS
    controller/boardmaster.h
    views/components/piecesprite.h
    common/position.h
    guimanager.h
    common/boardmove.h
    common/completemove.h
    chessengine.h
    views/components/piecesbimap.h
    resourcemanager.h
    controller/components/settingsstate.h
    views/moveview.h
    views/boardcanvas.h
    views/clocksview.h
    model/gamedata.h
    blackWolf.h
    views/statusview.h
    controller/components/buttonbox.h
    controller/components/sidechoicewindow.h
    views/components/firework.h
    client.h
    netwidgets.h
)

set(bw_SRCS
    main.cpp
    controller/boardmaster.cpp
    views/components/piecesprite.cpp
    common/position.cpp
    guimanager.cpp
    common/boardmove.cpp
    common/completemove.cpp
    chessengine.cpp
    views/components/piecesbimap.cpp
    ext/file_descriptor.cpp
    resourcemanager.cpp
    controller/components/settingsstate.cpp
    views/moveview.cpp
    views/boardcanvas.cpp
    views/clocksview.cpp
    model/gamedata.cpp
    blackWolf.cpp
    views/statusview.cpp
    controller/components/buttonbox.cpp
    controller/components/sidechoicewindow.cpp
    views/components/firework.cpp
    client.cpp
    netwidgets.cpp
)

set(bw_ALL ${bw_HDRS} ${bw_SRCS})

#Executable
add_executable(blackWolf ${bw_ALL})
target_link_libraries(blackWolf /usr/local/lib/libboost_system.so)
target_link_libraries(blackWolf /usr/local/lib/libsfgui.so)
target_link_libraries(blackWolf /usr/local/lib/libthor.so)
target_link_libraries(blackWolf /usr/local/lib/libsfml-graphics.so.2)
target_link_libraries(blackWolf /usr/local/lib/libsfml-window.so.2)
target_link_libraries(blackWolf /usr/local/lib/libsfml-system.so.2)
