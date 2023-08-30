#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
//#include "../output/output.h"

struct cityData {
    std::string city;
    double latitude;
    double longitude;
    std::string country;
    uint32_t population;
    bool is_capital;
};

struct WeatherInTime {
    std::string time;
    double precipitation;
    uint32_t relativehumidity_2m;
    int16_t temperature_2m;
    uint32_t visibility;
    uint8_t weather;
    double windspeed_10m;
    uint16_t winddirection;
};

bool getData(std::vector<WeatherInTime>& a, const std::string& city_name, uint16_t days);

bool readInfo(std::vector<std::vector<WeatherInTime>>& weather_list, std::vector<std::string>& cities, uint16_t& days);
void saveData(const std::vector<std::vector<WeatherInTime>>& weather_list, const std::vector<std::string>& cities, uint16_t days);
void loadData(std::vector<std::vector<WeatherInTime>>& weather_list, std::vector<std::string>& cities, uint16_t& days);
