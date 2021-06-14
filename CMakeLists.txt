cmake_minimum_required(VERSION 3.19)
project(Bejeweled00)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5Network REQUIRED)
include_directories(src)
link_libraries(ws2_32)
add_executable(Bejeweled00
        src/Server/Connect.cpp
        src/Server/Connect.h
        src/Impl/logic/board.cpp
        src/headers/logic/board.h
        src/Impl/components/timer.cpp
        src/headers/components/timer.h
        src/Impl/logic/game.cpp
        src/headers/logic/game.h
        src/Impl/logic/gamelogic.cpp
        src/headers/logic/gamelogic.h
        src/Impl/logic/gamestate.cpp
        src/headers/logic/gamestate.h
        src/Impl/ui/jewelbutton.cpp
        src/headers/ui/jewelbutton.h
        src/Impl/ui/jewel.cpp
        src/headers/ui/jewel.h
        src/main.cpp

        src/headers/ui/JoinRoompage.h
        src/headers/ui/Regiserpage.h
        src/headers/ui/JewelLineEdit.h

        src/Impl/ui/JoinRoompage.cpp
        src/Impl/ui/Regiserpage.cpp
        src/Impl/ui/JewelLineEdit.cpp

        src/Impl/ui/mainwindow.cpp
        src/headers/ui/mainwindow.h
        src/Impl/components/scoresystem.cpp
        src/headers/components/scoresystem.h
        src/Impl/ui/homepage.cpp
        src/headers/ui/homepage.h
        src/Impl/ui/selectpage.cpp
        src/headers/ui/selectpage.h
        src/Impl/ui/roompage.cpp
        src/headers/ui/roompage.h
        src/Impl/ui/scorepage.cpp
        src/headers/ui/scorepage.h
        src/Impl/ui/setpage.cpp
        src/headers/ui/setpage.h
        src/Impl/ui/qiplineedit.cpp
        src/headers/ui/qiplineedit.h src/headers/ui/aboutpage.h src/Impl/ui/aboutpage.cpp)

target_link_libraries(Bejeweled00 Qt5::Core)
target_link_libraries(Bejeweled00 Qt5::Widgets)
target_link_libraries(Bejeweled00 Qt5::Multimedia)
target_link_libraries(Bejeweled00 Qt5::Network)
