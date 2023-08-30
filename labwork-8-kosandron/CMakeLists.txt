cmake_minimum_required(VERSION 3.0.0)
project(lab8 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(labwork-8-kosandron bin/main.cpp lib/CircularBufferExt.h lib/CircularBuffer.h lib/Iterator.h lib/ConstIterator.h)

enable_testing()
add_subdirectory(tests)
