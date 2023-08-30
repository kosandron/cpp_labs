#include "parser/parser.h"
#include "heap/heap.h"
#include "bmp/bmp.h"

int main(int argc, char* argv[]) {
    //  reading command line
    std::vector<std::string> sources;
    for (int i = 1; i < argc; i++) {
        sources.push_back(argv[i]);
    }
    //  shows changed options
    Properties options = ReadOptions(argc, sources);
    List field;
    field.create(options);
    heapBreak(field, options);

    return 0;
}
