cmake_minimum_required(VERSION 3.28.3)
project(phantomboard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET
    gtk+-3.0
)

pkg_check_modules(APPINDICATOR REQUIRED IMPORTED_TARGET
    ayatana-appindicator3-0.1
)

find_package(sdbus-c++ REQUIRED)

add_executable(phantomboard-ui
    src/main.cpp
)

target_include_directories(phantomboard-ui
    PUBLIC ../include
)

target_link_libraries(phantomboard-ui
    PRIVATE PkgConfig::GTK3
            PkgConfig::APPINDICATOR
            SDBusCpp::sdbus-c++
)
