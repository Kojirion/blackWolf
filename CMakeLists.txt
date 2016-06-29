cmake_minimum_required(VERSION 2.8)

#project name
project(blackWolf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs") #suppress warning in Aurora in Thor

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Boost COMPONENTS system REQUIRED)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
find_package(SFGUI REQUIRED)
find_package(Thor REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})
include_directories(${THOR_INCLUDE_DIR})

set(bw_HDRS
    controller/Controller.hpp
    views/components/PieceSprite.hpp
    views/components/Emitter.hpp
    Application.hpp
    controller/components/Settings.hpp
    views/MoveList.hpp
    views/Canvas.hpp
    views/Clocks.hpp
    model/GameData.hpp
    BlackWolf.hpp
    views/Status.hpp
    controller/components/ButtonBox.hpp
    controller/components/ColorChoices.hpp
    Client.hpp
    Timeseal.hpp
    NetWidgets.hpp
    messages/Entity.hpp
    messages/Message.hpp
    messages/GameStart.hpp
    messages/GameState.hpp
    messages/GameEnd.hpp
    messages/TextReady.hpp
    messages/TextToClient.hpp
    parsers/GameStateParser.hpp
    parsers/SquareParser.hpp
    parsers/GameStartParser.hpp
    parsers/GameEndParser.hpp
    parsers/SessionStartParser.hpp
    parsers/Grammars.hpp
    generators/MoveStringGrammar.hpp
    Actions.hpp
)

set(bw_SRCS
    controller/Controller.cpp
    views/components/PieceSprite.cpp
    views/components/Emitter.cpp
    Application.cpp
    controller/components/Settings.cpp
    views/MoveList.cpp
    views/Canvas.cpp
    views/Clocks.cpp
    model/GameData.cpp
    BlackWolf.cpp
    views/Status.cpp
    controller/components/ButtonBox.cpp
    controller/components/ColorChoices.cpp
    Client.cpp
    Timeseal.cpp
    NetWidgets.cpp
    messages/Entity.cpp
    messages/Message.cpp
    messages/GameStart.cpp
    messages/GameState.cpp
    messages/GameEnd.cpp
    messages/TextReady.cpp
    messages/TextToClient.cpp
    parsers/GameStateParser.cpp
    parsers/GameStartParser.cpp
    parsers/SquareParser.cpp
    parsers/GameEndParser.cpp
    parsers/SessionStartParser.cpp
)

set(bw_ALL ${bw_HDRS} ${bw_SRCS})

#Executable
add_library(core ${bw_ALL})
target_link_libraries(core ${Boost_LIBRARIES})
target_link_libraries(core ${SFML_LIBRARIES})
target_link_libraries(core ${THOR_LIBRARY})
target_link_libraries(core ${SFGUI_LIBRARY})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} core)

#tests
set(BUILD_TESTS false CACHE BOOL "Build tests")

if (BUILD_TESTS)
    set(test_SRCS
        tests/tests.cpp
        tests/SessionStart.cpp
        tests/GameEnd.cpp
        tests/GameStart.cpp
        tests/GameState.cpp
        tests/Square.cpp
        tests/MoveString.cpp
    )
    add_executable(unit_tests ${test_SRCS})
    target_link_libraries(unit_tests core)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES resources/DejaVuSansMono.ttf DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES resources/BoardBlack.jpg DESTINATION ${CMAKE_INSTALL_PREFIX}/Graphics)
install(FILES resources/Pieces.png DESTINATION ${CMAKE_INSTALL_PREFIX}/Graphics)
install(FILES resources/gui.theme DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES resources/blackwolf.theme DESTINATION ${CMAKE_INSTALL_PREFIX})
