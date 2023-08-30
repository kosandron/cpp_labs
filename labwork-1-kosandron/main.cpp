#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct OutputProperties {
    bool show_lines = false;
    bool show_bytes = false;
    bool show_words = false;
    bool show_chars = false;
};

struct Statistic {
    size_t lines = 0;
    size_t bytes = 0;
    size_t words = 0;
    size_t symbol = 0;
};

void Reading(const std::string& filename, Statistic& stats) {
    char file_symbol;
    std::ifstream f(filename, std::ios::binary);
    char last = ' ';
    int symbols_in_string = 0;
    while (f.get(file_symbol)) { //  read file character by character
        stats.symbol++;
        symbols_in_string++;
        if (file_symbol == '\n') { //  end of line
            stats.lines++;
            symbols_in_string = 0;
        }
        if (std::isspace(file_symbol) && !std::isspace(last)) { //  end of word
            stats.words++;
        }
        stats.bytes++;
        last = file_symbol;
    }
    if (symbols_in_string > 0) {
        stats.lines++;
    }
    if (!std::isspace(last)) {
        stats.words++;
    }
}

void FileAnalysis(int argc, char* argv[], OutputProperties& options) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            Statistic stats;
            Reading(argv[i], stats);
            if (!options.show_lines && !options.show_bytes && !options.show_words && !options.show_chars) { //  print without options
                std::cout << stats.lines << " " << stats.words << " " << stats.bytes << " " << argv[i] << '\n';
            } else {
                if (options.show_lines) {
                    std::cout << stats.lines << " ";
                }
                if (options.show_bytes) {
                    std::cout << stats.bytes <<" ";
                }
                if (options.show_words) {
                    std::cout << stats.words << " ";
                }
                if (options.show_chars) {
                    std::cout << stats.symbol << " ";
                }
                std::cout << argv[i] << '\n';
            }
        }
    }
}

OutputProperties ReadOptions(int argc, std::vector <std::string>& sources) {
    OutputProperties options;
    for (int i = 0; i < argc-1; i++) {
        if (sources[i] == "-l" or sources[i] == "--lines") {
            options.show_lines = true;
        } else if (sources[i] == "-c" or sources[i] == "--bytes") {
            options.show_bytes = true;
        } else if (sources[i] == "-w" or sources[i] == "--words") {
            options.show_words = true;
        } else if (sources[i] == "-m" or sources[i] == "--chars") {
            options.show_chars = true;
        } else if (sources[i][0] == '-') {
            for (int j = 1;j < sources[i].size();j++) {
                if (sources[i][j] == 'l') {
                    options.show_lines = true;
                } else if (sources[i][j] == 'c') {
                    options.show_bytes = true;
                } else if (sources[i][j] == 'w') {
                    options.show_words = true;
                } else if (sources[i][j] == 'm') {
                    options.show_chars = true;
                } else {
                    std::cout << "error: illegal option -" << sources[i][j] << '\n';
                }
            }
        }
    }

    return options;
}

int main(int argc, char* argv[]) {
    //  reading command line
    std::vector <std::string> sources;
    for (int i = 1; i < argc; i++) {
        sources.push_back(argv[i]);
    }
    //  shows changed options
    OutputProperties options = ReadOptions(argc, sources);

    FileAnalysis(argc, argv, options);

    return 0;
}
