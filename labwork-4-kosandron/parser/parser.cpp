#include "parser.h"

#include <cstdint>

const uint16_t kFileRavnoSize = 7;
const uint16_t kControlBitesRavnoSize = 10;

Options ReadOptions(int argc, const char** argv) {
    Options parametrs;
    if (strcmp(argv[1], "--create") == 0 or argv[1][1] == 'c') {
        parametrs.create_archive = true;
    } else if (strcmp(argv[1], "--extract") == 0 or argv[1][1] == 'x') {
        parametrs.extract_files = true;
        std::cout << "Extract00\n";
    } else if (strcmp(argv[1], "--list") == 0 or argv[1][1] == 'l') {
        parametrs.list_of_files = true;
    } else if (strcmp(argv[1], "--append") == 0 or argv[1][1] == 'a') {
        parametrs.append_file = true;
    } else if (strcmp(argv[1], "--delete") == 0 or argv[1][1] == 'd') {
        parametrs.delete_file = true;
    } else if (strcmp(argv[1], "--concatenate") == 0 or argv[1][1] == 'A') {
        parametrs.concatenate_archives = true;
    }

    for (size_t i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-k") == 0) {
                parametrs.control_bites_count = std::atoi(argv[i + 1]);
                i++;
            } else if (strcmp(argv[i], "-f") == 0) {
                parametrs.archive_name = argv[i + 1];
                i++;
            } else {
                std::string temp = argv[i];
                if (temp.substr(0, kFileRavnoSize) == "--file=") {
                    parametrs.archive_name = temp.substr(kFileRavnoSize, temp.size() - kFileRavnoSize);
                } else if (temp.substr(0, kControlBitesRavnoSize) == "--kontrol=") {
                    parametrs.control_bites_count = std::stoi(temp.substr(kControlBitesRavnoSize, temp.size() - kControlBitesRavnoSize));
                }
            }
        } else {
            parametrs.files.emplace_back(argv[i]);
        }
    }

    return parametrs;
}