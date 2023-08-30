#include <iostream>
#include "parser/parser.h"

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cout << "Error! Too much or too few arguments!";
    } else {
        readInfo(argv[1]);
    }
    //readInfo("tag.mp3");
    return 0;
}
