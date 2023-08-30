#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Properties {
    uint16_t length;
    uint16_t width;
    std::string inputFile;
    std::string outputFile;
    uint64_t maxIter = UINT64_MAX;
    uint64_t freq = 0;
};

Properties ReadOptions(int argc, const std::vector<std::string> &sources);
