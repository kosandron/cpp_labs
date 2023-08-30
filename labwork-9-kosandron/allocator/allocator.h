#pragma once

#include "pool.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

template<typename T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    template<class U>
    struct rebind {
        typedef Allocator<U> other;
    };

private:
    std::vector<Pool<value_type>*> pools_;
public:
    explicit Allocator() {
        std::ifstream input(R"(..\\allocator\config.txt)");
        size_t n;
        input >> n;
        for (size_t i = 0; i < n; i++) {
            size_t a, b;
            input >> a >> b;
            pools_.push_back(new Pool<T>(a, b));
        }
    }

    pointer allocate(size_t n) {
        std::sort(pools_.begin(), pools_.end(), [n](const Pool<T> *lhs, const Pool<T> *rhs) {
            if (lhs->size() < n) {
                return false;
            }
            if (rhs->size() < n) {
                return true;
            }
            size_t delta_lhs = lhs->length() - (n % lhs->length());
            size_t delta_rhs = rhs->length() - (n % rhs->length());

            if (delta_lhs == delta_rhs) {
                return (lhs->length() > rhs->length());
            } else {
                return (delta_lhs < delta_rhs);
            }
        });


        for (Pool<value_type> *i: pools_) {
            pointer tmp = i->allocate(n);
            if (tmp != nullptr) {
                return tmp;
            }
        }
        throw std::bad_alloc{};
    }

    void deallocate(pointer tmp, size_t n) {
        for (Pool<value_type> *i: pools_) {
            if (i->deallocate(tmp, n)) {
                return;
            }
        }
        throw std::out_of_range("Pointer out of range");
    }

    ~Allocator() {
        for (Pool<value_type>* i: pools_) {
            delete i;
        }
    }

    std::vector<Pool<value_type>*> pools() const {
        return pools_;
    }
};

template<typename W, typename Z>
bool operator==(const Allocator<Z>& lhs, const Allocator<W>& rhs) {
    if (lhs.pools().size() != rhs.pools().size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.pools().size(); ++i) {
        if (lhs.pools()[i] != rhs.pools()[i]) {
            return false;
        }
    }

    return true;
}

template<typename W, typename Z>
bool operator!=(const Allocator<Z>& lhs, const Allocator<W>& rhs) {
    return !(lhs == rhs);
}