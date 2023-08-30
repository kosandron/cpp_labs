#include "Allocator.h"

#include <ctime>
#include <list>

int main() {
    std::vector<int, Allocator<int>> r;
    size_t time;
    time = clock();
    for (int i = 0; i < 100000; i++) {
        r.push_back(1);
    }
    std::cout << clock() - time << '\n';
    std::vector<int> d;
    time = clock();
    for (int i = 0; i < 100000; i++) {
        d.push_back(1);
    }
    std::cout << clock() - time;

    return 0;
}
