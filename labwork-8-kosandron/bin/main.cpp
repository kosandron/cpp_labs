#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

#include "../lib/CircularBufferExt.h"


int main() {
    CCircularBuffer<int> b(6);
    for (int i = 0; i < 6; i++) {
        b.push_back(6 - i);
    }
    std::sort(b.begin(), b.end());
    for (int i = 0; i < 6; i++) {
        std::cout << b[i] << " ";
    }
    std::cout << '\n';
    b.pop_before();
    for (ConstIterator i = b.cbegin(); i < b.cend(); i++) {
        std::cout << *i << " ";
    }
    std::cout << '\n';
    for (int i = 0; i < 6; i++) {
        std::cout << b[i] << " ";
    }
    /*b.pop_before();
    for (Iterator i = b.begin(); i < b.end(); i++) {
        std::cout << *i << " ";
    }*/
    std::cout << b.size() << " " << b.capacity();
    return 0;
}
