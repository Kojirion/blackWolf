cmake_minimum_required(VERSION 2.8)

#project name
project(blackWolf)

include_directories( ${CMAKE_SOURCE_DIR}/include )
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -pthread")

find_package(Boost COMPONENTS system REQUIRED)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
find_package(SFGUI REQUIRED)
find_package(Thor REQUIRED)

set(bw_HDRS
    controller/Controller.hpp
    views/components/PieceSprite.hpp
    common/Position.hpp
    Application.hpp
    common/PseudoMove.hpp
    common/CompleteMove.hpp
    Resources.hpp
    controller/components/Settings.hpp
    views/MoveList.hpp
    views/Canvas.hpp
    views/Clocks.hpp
    views/Counters.hpp
    model/GameData.hpp
    BlackWolf.hpp
    views/Status.hpp
    controller/components/ButtonBox.hpp
    controller/components/ColorChoices.hpp
    views/components/Firework.hpp
    Client.hpp
    NetWidgets.hpp
    Entity.hpp
    parsers/GameStateParser.hpp
    parsers/SquareParser.hpp
    parsers/GameStartParser.hpp
    parsers/GameEndParser.hpp
    parsers/SessionStartParser.hpp
    parsers/Grammars.hpp
)

set(bw_SRCS
    controller/Controller.cpp
    views/components/PieceSprite.cpp
    common/Position.cpp
    Application.cpp
    common/PseudoMove.cpp
    common/CompleteMove.cpp
    Resources.cpp
    controller/components/Settings.cpp
    views/MoveList.cpp
    views/Canvas.cpp
    views/Clocks.cpp
    views/Counters.cpp
    model/GameData.cpp
    BlackWolf.cpp
    views/Status.cpp
    controller/components/ButtonBox.cpp
    controller/components/ColorChoices.cpp
    views/components/Firework.cpp
    Client.cpp
    NetWidgets.cpp
    Entity.cpp
    parsers/GameStateParser.cpp
    parsers/GameStartParser.cpp
    parsers/SquareParser.cpp
    parsers/GameEndParser.cpp
    parsers/SessionStartParser.cpp
)

set(bw_ALL ${bw_HDRS} ${bw_SRCS})

#Executable
add_library(core ${bw_ALL})
target_link_libraries(core ${Boost_SYSTEM_LIBRARY})
target_link_libraries(core ${SFML_LIBRARIES})
target_link_libraries(core ${THOR_LIBRARY})
target_link_libraries(core ${SFGUI_LIBRARY})


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} core)

#tests
set(test_SRCS
    tests/tests.cpp
)
add_executable(test ${test_SRCS})
target_link_libraries(test core)
