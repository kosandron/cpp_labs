#include "data.h"

std::string URL_CITY = "https://api.api-ninjas.com/v1/city?name=";
std::string URL_WEATHER =  "https://api.open-meteo.com/v1/forecast";
std::string API_KEY = "z74N0B1YOnppRUfECA/nmw==Nirg2q1hBUeb1gSk";
bool error_is_printed = false;
using json = nlohmann::json;

bool getData(std::vector<WeatherInTime>& a, const std::string& city_name, uint16_t days) {
    cpr::Response r = cpr::Get(cpr::Url{URL_CITY + city_name},
                               cpr::Header{{"X-Api-Key", API_KEY}});
    if (r.status_code == 0) {
        if (!error_is_printed) {
            std::cerr << r.error.message << std::endl;
            error_is_printed = true;
        }
        return false;
    } else if (r.status_code >= 400) {
        if (!error_is_printed) {
            std::cerr << "Error [" << r.status_code << "] making request!" << std::endl;
            error_is_printed = true;
        }
        return false;
    }
    json js{json::parse(r.text)};
    cityData cordinates;
    cordinates.city = city_name;
    cordinates.latitude = js[0][0]["latitude"];
    cordinates.longitude = js[0][0]["longitude"];
    cordinates.country = js[0][0]["country"];
    r = cpr::Get(cpr::Url{URL_WEATHER}, cpr::Parameters{
            {"latitude",        std::to_string(cordinates.latitude)},
            {"longitude",       std::to_string(cordinates.longitude)},
            {"current_weather", "true"},
            {"forecast_days",   std::to_string(days)},
            {"hourly",          "temperature_2m"},
            {"hourly",          "winddirection_10m"},
            {"hourly",          "windspeed_10m"},
            {"hourly",          "relativehumidity_2m"},
            {"hourly",          "visibility"},
            {"hourly",          "precipitation"},
            {"hourly",          "weathercode"},
    });
    if (r.status_code == 0) {
        if (!error_is_printed) {
            std::cerr << r.error.message << std::endl;
            error_is_printed = true;
        }
        return false;
    } else if (r.status_code >= 400) {
        if (!error_is_printed) {
            std::cerr << "Error [" << r.status_code << "] making request!" << std::endl;
            error_is_printed = true;
        }
        return false;
    }
    json p = json::parse(r.text);

    WeatherInTime temp;
    for (int i = 6; i < 24 * days; i += 6) {
        temp.precipitation = p["hourly"]["precipitation"][i];
        temp.relativehumidity_2m = p["hourly"]["relativehumidity_2m"][i];
        temp.temperature_2m = p["hourly"]["temperature_2m"][i];
        temp.time = static_cast<std::string>(p["hourly"]["time"][i]).substr(0, 10);
        temp.visibility = p["hourly"]["visibility"][i];
        temp.weather = p["hourly"]["weathercode"][i];
        temp.windspeed_10m = p["hourly"]["windspeed_10m"][i];
        temp.winddirection = p["hourly"]["winddirection_10m"][i];
        a.push_back(temp);
    }
    temp.precipitation = p["hourly"]["precipitation"][24 * days- 1];
    temp.relativehumidity_2m = p["hourly"]["relativehumidity_2m"][24 * days- 1];
    temp.temperature_2m = p["hourly"]["temperature_2m"][24 * days- 1];
    temp.time = static_cast<std::string>(p["hourly"]["time"][24 * days- 1]).substr(0, 10);
    temp.visibility = p["hourly"]["visibility"][24 * days- 1];
    temp.weather = p["hourly"]["weathercode"][24 * days- 1];
    temp.windspeed_10m = p["hourly"]["windspeed_10m"][24 * days - 1];
    temp.winddirection = p["hourly"]["winddirection_10m"][24 * days- 1];
    a.push_back(temp);

    return true;
}

bool readInfo(std::vector<std::vector<WeatherInTime>>& weather_list, std::vector<std::string>& cities, uint16_t& days) {
    std::vector<WeatherInTime> tmp;
    bool internet_status = true;

    for (size_t i = 0; i < cities.size(); i++) {
        internet_status = getData(tmp, cities[i], days);

        if (internet_status) {
            weather_list.push_back(tmp);
            tmp.clear();
        } else {
            if (std::filesystem::exists("cash.txt")) {
                try {
                    loadData(weather_list, cities, days);
                }
                catch (std::exception& e) {
                    system("cls");
                    std::cout << "You don`t have Internet and your cash-file is broken! Bad parsing of save file! Now program cannot work without Internet-connection!\n";
                    system("pause");
                    exit(0);
                    //throw std::string("Cash error");
                    //std::cout << e.what() << '\n';
                }
                return internet_status;
            } else {
                std::cout << "There is NOT cash file, because program has not ever worked with Internet-connection!\n";
                return internet_status;
            }
        }
    }
    saveData(weather_list, cities, days);

    return internet_status;
}

void saveData(const std::vector<std::vector<WeatherInTime>>& weather_list, const std::vector<std::string>& cities, uint16_t days) {
    std::ofstream f("cash.txt");
    f << "{\n\"cities\": [\n";
    for (size_t i = 0; i < cities.size() - 1; i++) {
        f << '\t' << '\"' << cities[i] << '\"' << ',' << '\n';
    }
    f << '\t' << '\"' << cities[cities.size() - 1] << '\"' << '\n';
    f << "],\n";
    f << "\"days\": " << ' ' << days << ',' << '\n';

    for (size_t i = 0; i < cities.size(); i++) {
        f << '\"' << cities[i] << "\": {\n";
        f << "\t\"precipitation\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].precipitation) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].precipitation) << "\n";
        f << "\t],\n";

        f << "\t\"relativehumidity_2m\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].relativehumidity_2m) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].relativehumidity_2m) << "\n";
        f << "\t],\n";

        f << "\t\"temperature_2m\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].temperature_2m) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].temperature_2m) << "\n";
        f << "\t],\n";

        f << "\t\"time\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << '\"' << weather_list[i][j].time << '\"' << ",\n";
        f << '\t' << '\t' << '\"' << weather_list[i][weather_list[i].size() - 1].time << '\"' << "\n";
        f << "\t],\n";

        f << "\t\"visibility\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].visibility) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].visibility) << "\n";
        f << "\t],\n";

        f << "\t\"weather\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].weather) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].weather) << "\n";
        f << "\t],\n";

        f << "\t\"windspeed_10m\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].windspeed_10m) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].windspeed_10m) << "\n";
        f << "\t\t],\n";

        f << "\t\"winddirection\": [\n";
        for (size_t j = 0; j < weather_list[i].size() - 1; j++)
            f << '\t' << '\t' << std::to_string(weather_list[i][j].winddirection) << ",\n";
        f << '\t' << '\t' << std::to_string(weather_list[i][weather_list[i].size() - 1].winddirection) << "\n";
        f << "\t]\n";
        if (i != cities.size() - 1)
            f << "},\n";
        else
            f << "}\n";
    }
    f << '}';
    f.close();
}

void loadData(std::vector<std::vector<WeatherInTime>>& weather_list, std::vector<std::string>& cities, uint16_t& days) {
    std::ifstream f("cash.txt");
    json p{json::parse(f)};
    cities = p[0]["cities"];
    days = p[0]["days"];

    weather_list.resize(cities.size());
    std::vector<WeatherInTime> tmp_city;
    WeatherInTime temp;
    for (size_t i = 0; i < cities.size(); i++) {
        for (size_t j = 0; j < days * 4; j++) {
            temp.precipitation = p[0][cities[i]]["precipitation"][j];
            temp.relativehumidity_2m = p[0][cities[i]]["relativehumidity_2m"][j];
            temp.temperature_2m = p[0][cities[i]]["temperature_2m"][j];
            temp.time = (p[0][cities[i]]["time"][j]);
            temp.visibility = p[0][cities[i]]["visibility"][j];
            temp.weather = p[0][cities[i]]["weather"][j];
            temp.windspeed_10m = p[0][cities[i]]["windspeed_10m"][j];
            temp.winddirection = p[0][cities[i]]["winddirection"][j];
            weather_list[i].push_back(temp);
        }
    }
}