#include "output.h"

Element sun() {
    return vbox({
                        color(Color::Gold1, text("     \\   /    ")),
                        color(Color::Gold1, text("      .-.      ")),
                        color(Color::Gold1, text("   ― (   ) ―  ")),
                        color(Color::Gold1, text("      `-’      ")),
                        color(Color::Gold1, text("     /   \\    ")),
                });
}

Element fog() {
    return vbox({
                        color(Color::SteelBlue, text("               ")),
                        color(Color::SteelBlue, text("  _ - _ - _ -  ")),
                        color(Color::SteelBlue, text("   _ - _ - _   ")),
                        color(Color::SteelBlue, text("  _ - _ - _ -  ")),
                        color(Color::SteelBlue, text("               ")),
                });
}

Element cloud() {
    return vbox({
                        color(Color::White, text("               ")),
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::White, text("               ")),
                });
}

Element sun_cloud() {
    return vbox({
                        color(Color::Gold1, text("    \\  /       ")),
                        hbox({color(Color::Gold1, text("  _ /\"\"")), color(Color::White, text(".-.     "))}),
                        hbox({color(Color::Gold1, text("    \\_")), color(Color::White, text("(   ).   "))}),
                        hbox({color(Color::Gold1, text("    /")), color(Color::White, text("(___(__)  "))}),
                        color(Color::White, text("               ")),
                });
}

Element snow() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::White, text(" *  *  *  *    ")),
                        color(Color::White, text("*  *  *  *     ")),
                });
}

Element rain() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::DeepSkyBlue2, text(" '  ' ' ' ' '  ")),
                        color(Color::DeepSkyBlue2, text(" ' ' ' ' ' '   ")),
                });
}

Element big_rain() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::DeepSkyBlue2, text(" ''''''''''''' ")),
                        color(Color::DeepSkyBlue2, text(" ''''''''''''' ")),
                });
}

Element rain_snow() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::DeepSkyBlue2, text("  * ' * ' * '  ")),
                        color(Color::DeepSkyBlue2, text(" ' * ' * ' *   ")),
                });
}

Element big_snow() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        color(Color::DeepSkyBlue2, text(" ************* ")),
                        color(Color::DeepSkyBlue2, text(" ************* ")),
                });
}

Element thunderstorm() {
    return vbox({
                        color(Color::White, text("      .--.     ")),
                        color(Color::White, text("   .-(    ).   ")),
                        color(Color::White, text("  (___.__)__)  ")),
                        hbox({color(Color::DeepSkyBlue2, text(" '''''")), color(Color::Yellow1, text("/")), color(Color::DeepSkyBlue2, text("''''''' "))}),
                        hbox({color(Color::DeepSkyBlue2, text(" '''''")), color(Color::Yellow1, text("\\")), color(Color::DeepSkyBlue2, text("''''''' "))}),
                });
}

Element paint(uint8_t weathercode) {
    if (weathercode == 0 or weathercode == 1) {
        return sun();
    } else if (weathercode == 3) {
        return cloud();
    } else if (weathercode == 2) {
        return sun_cloud();
    } else if (weathercode == 45 or weathercode == 48) {
        return fog();
    } else if (weathercode > 50 and weathercode < 66) {
        return rain();
    } else if (weathercode == 66 or weathercode == 67) {
        return rain_snow();
    } else if (weathercode > 70 and weathercode < 78) {
        return snow();
    } else if (weathercode > 79 and weathercode < 83) {
        return big_rain();
    } else if (weathercode == 84 or weathercode == 85) {
        return big_snow();
    } else {
        return thunderstorm();
    }
}
Element box_construct(const std::string& daytime, const std::string& weather, const std::string& temperature, const std::string& wind, const std::string& visibility, const std::string& humidity, uint8_t weather_code) {
    return vbox({
        text(daytime) | center,
        separator(),
        hbox({
            paint(weather_code),
            vbox({
                    text(weather),
                    text(temperature),
                    text(wind),
                    text(visibility),
                    text(humidity),
            }),
        }),
    });
}

Element day_construct(const std::vector<WeatherInTime>& a, uint16_t day) {
    return vbox(border(text(a[day * 4].time)) | center,
                hbox({
                        box_construct("Morning", weatherCode(a[day * 4].weather), temperature(a[day * 4].temperature_2m),
                                      windLine(a[day * 4].winddirection, a[day * 4].windspeed_10m), visibility(a[day * 4].visibility),  water(a[day * 4].precipitation, a[day * 4].relativehumidity_2m), a[day * 4].weather),
                        separator(),
                        box_construct("Afternoon", weatherCode(a[day*4 + 1].weather), temperature(a[day*4 + 1].temperature_2m),
                                      windLine(a[day * 4 + 1].winddirection, a[day * 4 + 1].windspeed_10m), visibility(a[day * 4 + 1].visibility), water(a[day * 4 + 1].precipitation, a[day * 4 + 1].relativehumidity_2m), a[day * 4 + 1].weather),
                        separator(),
                        box_construct("Evening", weatherCode(a[day * 4 + 2].weather), temperature(a[day*4 + 2].temperature_2m),
                        windLine(a[day * 4 + 2].winddirection, a[day * 4 + 2].windspeed_10m),  visibility(a[day * 4 + 2].visibility), water(a[day * 4 + 2].precipitation, a[day * 4].relativehumidity_2m), a[day * 4 + 2].weather),
                        separator(),
                        box_construct("Night", weatherCode(a[day * 4 + 3].weather), temperature(a[day * 4 + 3].temperature_2m),
                        windLine(a[day * 4 + 3].winddirection, a[day * 4 + 3].windspeed_10m),  visibility(a[day * 4 + 3].visibility), water(a[day * 4 + 3].precipitation, a[day * 4 + 3].relativehumidity_2m), a[day * 4 + 3].weather),
    }) | borderStyled(ROUNDED));
}

void showWeather(uint16_t days, uint16_t city_number, const std::vector<std::vector<WeatherInTime>>& list, const std::string& city) {
    system("cls");
    std::cout << "City: " << city << '\n';
    for (uint16_t i = 0; i < days; i++) {
        auto document = day_construct(list[city_number], i);
        auto screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
        Render(screen, document);
        screen.Print();
        std::cout << std::endl;
    }
}


const std::string weatherCode(uint8_t code) {
    if (code == 0) {
        return "Clear sky";
    } else if (code == 1) {
        return "Mainly clear";
    } else if (code == 2) {
        return "Partly cloudy";
    } else if (code == 3) {
        return "Overcast";
    } else if (code == 45) {
        return "Fog";
    } else if (code == 48) {
        return "Depositing rime fog";
    } else if (code == 51) {
        return "Light drizzle";
    } else if (code == 53) {
        return "Moderate drizzle";
    } else if (code == 55) {
        return "Dense intensity drizzle";
    } else if (code == 56) {
        return "Light freezing drizzle";
    } else if (code == 57) {
        return "Dense intensity freezing drizzle";
    } else if (code == 61) {
        return "Slight rain";
    } else if (code == 63) {
        return "Moderate rain";
    } else if (code == 65) {
        return "Heavy intensity rain";
    } else if (code == 66) {
        return "Light freezing rain";
    } else if (code == 67) {
        return "Heavy intensity freezing rain";
    } else if (code == 71) {
        return "Slight snow fall";
    } else if (code == 73) {
        return "Moderate snow fall";
    } else if (code == 75) {
        return "Heavy intensity snow fall";
    } else if (code == 77) {
        return "Snow grains";
    } else if (code == 80) {
        return "Slight rain showers";
    } else if (code == 81) {
        return "Moderate rain showers";
    } else if (code == 82) {
        return "Violent rain showers";
    } else if (code == 85) {
        return "Slight snow showers";
    } else if (code == 86) {
        return "Heavy snow showers";
    } else if (code == 95) {
        return "Thunderstorm";
    } else if (code == 96) {
        return "Thunderstorm with slight";
    } else if (code == 99) {
        return "Thunderstorm with heavy hail";
    }
}

const std::string windLine(uint16_t gradus, uint16_t windspeed) {
    gradus = gradus % 360;
    int temp_min = INT_MAX, index = 0;
    for (int i = 0; i < 8; i++) {
        if (abs(i*45 - gradus) < temp_min) {
            temp_min = abs(i*45 - gradus);
            index = i;
        }
    }

    if (index == 0) {
        return "↑" + std::to_string(windspeed) + " km/h";
    } else if (index == 1) {
        return "↗" + std::to_string(windspeed) + " km/h";
    } else if (index == 2) {
        return "→" + std::to_string(windspeed) + " km/h";
    } else if (index == 3) {
        return "↘" + std::to_string(windspeed) + " km/h";
    } else if (index == 4) {
        return "↓" + std::to_string(windspeed) + " km/h";
    } else if (index == 5) {
        return "↙" + std::to_string(windspeed) + " km/h";
    } else if (index == 6) {
        return "←" + std::to_string(windspeed) + " km/h";
    } else if (index == 7) {
        return "↖" + std::to_string(windspeed) + " km/h";
    }
}

const std::string temperature(int16_t temp) {
    if (temp > 0) {
        return '+' + std::to_string(temp) + " °C";
    }
    return std::to_string(temp) + " °C";
}

const std::string visibility(int32_t dist) {
    return std::to_string(dist / 1000) + " km";
}

const std::string water(double precipitation, uint32_t relativehumidity) {
    return std::to_string(precipitation).substr(0, 3) + " mm | " + std::to_string(relativehumidity) + '%';
}