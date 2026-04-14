cmake_minimum_required(VERSION 3.28.3)
project(phantomboard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(sdbus-c++ REQUIRED)

add_executable(phantomboard-ctl
    src/main.cpp
    src/app.cpp
    src/dbus/dbus_client.cpp
)

target_include_directories(phantomboard-ctl
    PRIVATE include
            ../include
)

target_link_libraries(phantomboard-ctl
    PRIVATE SDBusCpp::sdbus-c++
)
