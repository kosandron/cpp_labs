#include "lib/lib.h"
#include <vector>
#include <list>

int main() {
    /*auto a = xrange(1, 10);
    for (auto i : a) {
        std::cout << i << ' ';
    }
    for (auto i : xrange(1, 10, 2)) {
        std::cout << i << " ";
    }*/
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    //zip(l, v) a;
    for(auto value : zip(l, v)) {
        std::cout << value.first << " " << value.second << std::endl;
    }
    return 0;
}