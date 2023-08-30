#pragma once

#include "../archivator/archivator.h"

#include <cstring>
#include <string>
#include <vector>

struct Options {
    bool create_archive = false;
    bool list_of_files = false;
    bool extract_files = false;
    bool append_file = false;
    bool delete_file = false;
    bool concatenate_archives = false;
    uint8_t control_bites_count = 0;
    std::vector<std::string> files{};
    std::string archive_name = "";
};

Options ReadOptions(int argc, const char** argv);
