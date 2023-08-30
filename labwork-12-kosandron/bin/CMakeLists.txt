add_executable(labwork_12_kosandron main.cpp)

target_link_libraries(labwork_12_kosandron MyCoolDB)
target_include_directories(labwork_12_kosandron PRIVATE ${PROJECT_SOURCE_DIR})