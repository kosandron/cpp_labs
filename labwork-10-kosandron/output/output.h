#pragma once

#include "../data/data.h"
#include <ftxui/dom/elements.hpp>  // for text, operator|, border, Element, vbox, center, Fit, dbox
#include <ftxui/screen/screen.hpp>  // for Full, Screen
#include <memory>                   // for allocator

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"

using namespace ftxui;

Element box_construct(const std::string& daytime, const std::string& weather, const std::string& temperature, const std::string& wind, uint32_t visibility, const std::string& humidity, uint8_t weather_code);
Element day_construct(const std::vector<WeatherInTime>& a, uint16_t day);
void showWeather(uint16_t days, uint16_t city_number, const std::vector<std::vector<WeatherInTime>>& list, const std::string& city);

const std::string weatherCode(uint8_t code);
const std::string windLine(uint16_t gradus, uint16_t windspeed);
const std::string temperature(int16_t temp);
const std::string visibility(int32_t dist);
const std::string water(double precipitation, uint32_t relativehumidity);