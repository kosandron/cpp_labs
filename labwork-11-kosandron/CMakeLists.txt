cmake_minimum_required(VERSION 3.23)
project(labwork_11_kosandron)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(lib)
add_executable(labwork_11_kosandron main.cpp lib/lib.h)

enable_testing()
add_subdirectory(tests)