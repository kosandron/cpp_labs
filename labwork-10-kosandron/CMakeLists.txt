cmake_minimum_required(VERSION 3.23)
project(labwork_10_kosandron)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

#find_package(REQUIRED COMPONENTS Core Gui Widgets)

include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr GIT_TAG 1.10.2)
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json GIT_TAG v3.11.2)
FetchContent_Declare(ftxui GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui GIT_TAG v4.0.0)
FetchContent_MakeAvailable(cpr json ftxui)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(data)
add_subdirectory(output)
add_executable(labwork_10_kosandron main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC data)
target_link_libraries(${PROJECT_NAME} PUBLIC output)

target_link_libraries(labwork_10_kosandron PUBLIC ftxui::dom ftxui::component ftxui::screen)
target_link_libraries(labwork_10_kosandron PUBLIC cpr::cpr)
target_link_libraries(labwork_10_kosandron PUBLIC nlohmann_json::nlohmann_json)
#target_link_libraries(${PROJECT_NAME} PRIVATE bin)