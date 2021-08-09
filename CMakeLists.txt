cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(gps LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (UNIX)

EXECUTE_PROCESS(COMMAND uname -m OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE architecture)
if (architecture STREQUAL "aarch64")

add_executable(gps_unix app/main.cpp
        app/serial_linux.h
        app/serial_linux.cpp)

else ()

add_executable(gps_aarch64 app/main.cpp
        app/serial_linux.h
        app/serial_linux.cpp)

endif ()

endif ()
