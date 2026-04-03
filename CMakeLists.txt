cmake_minimum_required(VERSION 3.28.3)
project(bunnyboard LANGUAGES CXX)

add_library(bunnyboard_daemon
    src/daemon/buffer_manager.cpp
    src/daemon/daemon.cpp
    src/daemon/input_manager.cpp
    src/daemon/output_manager.cpp
    src/daemon/state_manager.cpp
)

target_include_directories(bunnyboard_daemon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/daemon
)

target_compile_features(bunnyboard_daemon PUBLIC cxx_std_17)

add_executable(bunnyboard-daemon
    src/daemon/main.cpp
)

target_link_libraries(bunnyboard-daemon
    PRIVATE bunnyboard_daemon
)
