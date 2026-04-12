cmake_minimum_required(VERSION 3.28.3)
project(phantomboard LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET
    glib-2.0
    gio-2.0
    gobject-2.0
)

pkg_check_modules(IBUS REQUIRED IMPORTED_TARGET
    ibus-1.0
)

add_library(phantomboard_daemon
    src/core/buffer_manager.cpp
    src/core/daemon.cpp
    src/core/input_manager.cpp
    src/core/output_manager.cpp
    src/core/state_manager.cpp
    src/ime/ibus/ibus_client.cpp
    src/ime/ibus/ibus_callbacks.cpp
)

target_include_directories(phantomboard_daemon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(phantomboard_daemon
    PUBLIC  PkgConfig::GLIB
            PkgConfig::IBUS
)

add_executable(phantomboard-daemon
    src/main.cpp
)

target_link_libraries(phantomboard-daemon
    PRIVATE phantomboard_daemon
)
