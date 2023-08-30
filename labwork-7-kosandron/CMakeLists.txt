cmake_minimum_required(VERSION 3.23)
project(labwork_7_kosandron)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(parser)
add_executable(labwork_7_kosandron main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)