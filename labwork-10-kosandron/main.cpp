#include <conio.h>
#include <windows.h>

#include "output/output.h"
#include "data/data.h"
using json = nlohmann::json;

struct Config {
    std::vector<std::string> cities;
    uint16_t days;
    uint64_t update_frequency;
    Config() {
        std::ifstream readfile("config.txt");
        json p{json::parse(readfile)};
        cities = p[0]["cities"];
        days = p[0]["days"];
        update_frequency = p[0]["update_frequency"];
    }
    void write() {
        std::ofstream file("config.txt");
        file << "{\"cities\": [";
        for (size_t i = 0; i < cities.size() - 1; i++) {
            file << '\"' << cities[i] << "\", ";
        }
        file << '\"' << cities[cities.size() - 1] << "\"], \"days\": " << days << ", \"update_frequency\": " << update_frequency << '}';
    }
};


int main(int argc, char *argv[]) {
    Config* list = new Config();
    std::vector<std::string> now_list_of_cities = list->cities;
    uint16_t now_days = list->days;
    std::vector<std::vector<WeatherInTime>> list_of_weather;
    list_of_weather.reserve(list->cities.size());
    bool new_internet_status = false;
    //HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    new_internet_status = readInfo(list_of_weather, now_list_of_cities, now_days);
    if (new_internet_status)
        std::cout << "Yes internet";
    else
        std::cout << "No internet";
    uint16_t number_of_city = 0, days = 1;
    showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);

    bool color = false;
    uint64_t time_tmp = list->update_frequency;
    while (true) {
        int command = getch();
        if (clock() / (1000 * list->update_frequency) >= time_tmp) {
            new_internet_status = readInfo(list_of_weather, list->cities, list->days);
            if (new_internet_status) {
                now_list_of_cities = list->cities;
                now_days = list->days;
            }
            time_tmp += list->update_frequency;
            showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
            //std::cout << 5;
        }
        if (command == 27) {
            if (new_internet_status)
                saveData(list_of_weather, list->cities, list->days);
            list->write();
            break;
        }
        if (command == 110) {
            number_of_city = (number_of_city + 1) % now_list_of_cities.size();
            showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
        }
        if (command == 112) {
            number_of_city = (number_of_city - 1 + now_list_of_cities.size()) % now_list_of_cities.size();
            showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
        }
        if (command == 43) {
            if (days < now_days) {
                days++;
                showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
            }
        }
        if (command == 45) {
            if (days > 1) {
                days--;
                showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
            }
        }

        if (command == 115) {
            uint8_t error = 0;
            std::string new_city;
            do {
                system("cls");
                if (error == 1) {
                    std::cout << "You entered returned city!" << '\n';
                } else if (error == 2) {
                    std::cout << "You have error in name of city!" << '\n';
                }
                error = 0;
                std::cout << "List of now cities: " << '\n';
                for (size_t i = 0; i < now_list_of_cities.size(); i++) {
                    std::cout << i + 1 << ". " << now_list_of_cities[i] << '\n';
                }

                std::cout << "If you do not want to add city, print \'0\'. Enter new city: " << '\n';
                std::cin >> new_city;
                if (new_city == "0") {
                    break;
                }
                for (size_t i = 0; i < now_list_of_cities.size(); i++) {
                    if (new_city == now_list_of_cities[i]) {
                        error = 1;
                        break;
                    }
                }

                cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city?name=" + new_city},
                                           cpr::Header{{"X-Api-Key", "z74N0B1YOnppRUfECA/nmw==Nirg2q1hBUeb1gSk"}});
                if (r.status_code == 0) {
                    error = 2;
                } else if (r.status_code >= 400) {
                    error = 2;
                }
                if (r.text == "[]") {
                    error = 2;
                }

            } while (error != 0);
            if (new_city != "0") {
                list->cities.push_back(new_city);
            }
            list_of_weather.clear();
            new_internet_status = readInfo(list_of_weather, list->cities, list->days);
            if (new_internet_status) {
                now_list_of_cities = list->cities;
                now_days = list->days;
            }
            showWeather(days, number_of_city, list_of_weather, now_list_of_cities[number_of_city]);
        }
    }

    return 0;
}
