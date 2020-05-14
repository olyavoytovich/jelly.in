cmake_minimum_required(VERSION 3.10)
project(jellyin)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON) # Enable Qt MOC compiler for managing signals / slots
set(CMAKE_AUTORCC ON) # Enable Qt resources compiler
set(CMAKE_AUTOUIC ON) # Enable Qt UI compiler
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_PREFIX_PATH "~/Qt/5.13.2/gcc_64/lib/cmake")  # Linux
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.13.0/5.13.0/mingw73_64/lib/cmake")  # Windows

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)

file(GLOB box2d
        box2d/*/*.cpp)

add_executable(jellyin main.cpp
        Controller/game_controller.cpp
        Model/animation.cpp
        Model/animator.cpp
        Model/constants.h
        Model/contact_listener.cpp
        Model/entity.cpp
        Model/game_object.cpp
        Model/map.cpp
        Model/map_loader.cpp
        Model/patroller.cpp
        Model/player.cpp
        Model/pressure_plate.cpp
        Model/shooter.cpp
        Model/sound_manager.cpp
        View/button.cpp
        View/choose_level_menu.cpp
        View/game_interface.cpp
        View/intermediate_menu.cpp
        View/label.cpp
        View/main_menu.cpp
        View/menu.cpp
        View/movie.cpp
        View/view.cpp
        resources.qrc
        ${box2d})

target_link_libraries(jellyin Qt5::Core)
target_link_libraries(jellyin Qt5::Widgets)
target_link_libraries(jellyin Qt5::Multimedia)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer -g")
