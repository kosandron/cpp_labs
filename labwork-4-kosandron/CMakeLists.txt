cmake_minimum_required(VERSION 3.23)
project(labwork_4_kosandron)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(parser)
add_subdirectory(archivator)
add_subdirectory(coding)
add_executable(labwork_4_kosandron main.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static")
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE archivator)
target_link_libraries(${PROJECT_NAME} PRIVATE coding)
