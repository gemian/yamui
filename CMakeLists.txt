cmake_minimum_required(VERSION 3.12)

project(yamui)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(minui)

add_executable(yamui
        yamui.cpp
        )

target_link_libraries(yamui
        libminui)

add_executable(yamui-powerkey
        yamui-tools.cpp
        yamui-powerkey.cpp
        )

include_directories(yamui-powerkey
        /usr/include/libdrm)

add_executable(yamui-screensaverd
        yamui-tools.cpp
        yamui-screensaverd.cpp
        )

target_link_libraries(yamui-screensaverd
        libminui)
