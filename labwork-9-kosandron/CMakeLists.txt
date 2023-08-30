cmake_minimum_required(VERSION 3.23)
project(labwork_9_kosandron)

set(CMAKE_CXX_STANDARD 17)

include_directories(allocator)

add_executable(labwork-9-kosandron main.cpp
        allocator/pool.h
        allocator/allocator.h)