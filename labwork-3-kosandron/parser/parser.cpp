#include "parser.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

Properties ReadOptions(int argc,  const std::vector<std::string> &sources) {
    Properties options;
    for (int i = 0; i < argc - 1; i += 2) {
        if (sources[i] == "-l" or sources[i] == "--length") {
            options.length = std::stoi(sources[i + 1]);
        } else if (sources[i] == "-w" or sources[i] == "--width") {
            options.width = std::stoi(sources[i + 1]);
        } else if (sources[i] == "-i" or sources[i] == "--input") {
            options.inputFile = sources[i + 1];
        } else if (sources[i] == "-o" or sources[i] == "--output") {
            options.outputFile = sources[i + 1];
        } else if (sources[i] == "-m" or sources[i] == "--max-iter") {
            options.maxIter = std::stoi(sources[i + 1]);
        } else if (sources[i] == "-f" or sources[i] == "--freq") {
            options.freq = std::stoi(sources[i + 1]);
        } else {
            std::cout << "error: illegal option -" << sources[i] << '\n';
        }
    }

    return options;
}
