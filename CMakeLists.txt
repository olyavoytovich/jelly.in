cmake_minimum_required(VERSION 3.10)
project(jellyin)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON) # Enable Qt MOC compiler for managing signals / slots
set(CMAKE_AUTORCC ON) # Enable Qt resources compiler
set(CMAKE_AUTOUIC ON) # Enable Qt UI compiler
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "~/Qt/5.13.2/gcc_64/lib/cmake")  # Linux
#set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.13.0/5.13.0/mingw73_64/lib/cmake")  # Windows

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

file(GLOB box2d
        src/box2d/*/*.cpp)

add_executable(jellyin main.cpp
        game_controller.cpp
        game_object.cpp
        view.cpp
        entity.cpp
        ${box2d})

target_link_libraries(jellyin Qt5::Core)
target_link_libraries(jellyin Qt5::Widgets)
