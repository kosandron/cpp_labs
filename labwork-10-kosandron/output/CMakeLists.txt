add_library(output output.h output.cpp)
target_link_libraries(output ftxui::dom ftxui::component ftxui::screen)
target_link_libraries(output cpr::cpr)
target_link_libraries(output nlohmann_json::nlohmann_json)