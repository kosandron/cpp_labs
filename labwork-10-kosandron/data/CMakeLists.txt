add_library(data data.cpp)
target_link_libraries(data cpr::cpr)
target_link_libraries(data nlohmann_json::nlohmann_json)