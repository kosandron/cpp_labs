cmake_minimum_required(VERSION 3.23)
project(labwork_12_kosandron)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)

#include_directories(MyCoolDB)
#include_directories(parser)
#add_executable(labwork_12_kosandron main.cpp MyCoolDB/MyCoolDB.h MyCoolDB/MyCoolDB.cpp
#        MyCoolDB/Column.h
#        MyCoolDB/DBExeptions.h MyCoolDB/table.h
 #        )
link_directories(MyCoolDB)

add_subdirectory(MyCoolDB)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
