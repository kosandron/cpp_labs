#pragma once

#include <vector>
#include <cmath>
#include <cstdint>

template<typename T>
class Pool {
private:
    std::vector<std::pair<size_t, size_t>> free_;
    T* start_;
    size_t count_of_chanks_in_pool_;
    size_t number_of_elements_in_chank_;
    size_t size_;
public:
    explicit Pool(size_t count, size_t length) {
        count_of_chanks_in_pool_ = count;
        number_of_elements_in_chank_ = length;
        size_ = count_of_chanks_in_pool_ * number_of_elements_in_chank_;
        start_ = static_cast<T*>(operator new(size_ * sizeof(T)));
        free_.push_back(std::make_pair(0, count));
    }

    T* allocate(size_t n) {
        size_t count_pieces;

        if (n % number_of_elements_in_chank_ != 0) {
            count_pieces = n / number_of_elements_in_chank_ + 1;
        } else {
            count_pieces = n / number_of_elements_in_chank_;
        }

        size_t free_index = 0;

        for (int i = 0; i < free_.size(); i++) {
            auto tmp = free_[i];
            if (tmp.second >= count_pieces) {
                free_index = tmp.first;
                if (tmp.second == count_pieces) {
                    free_.erase(free_.begin() + i);
                } else {
                    free_[i].first += count_pieces;
                    free_[i].second -= count_pieces;
                }
                return start_ + free_index * number_of_elements_in_chank_;
            }
        }

        return nullptr;
    }

    bool deallocate(T* tmp, size_t n) {
        if (tmp >= start_ && tmp <= (start_ + size_)) {
            size_t index = tmp - start_;
            free_.emplace_back(index, n);
            update_free(index, n);
            return true;
        } else {
            return false;
        }
    }

    size_t size() const {
        return size_;
    }

    size_t count() const {
        return count_of_chanks_in_pool_;
    }

    size_t length() const {
        return number_of_elements_in_chank_;
    }

    T* start() const {
        return start_;
    }

    std::vector<std::pair<size_t, size_t>> free() const {
        return free_;
    }

    void update_free(size_t index, size_t n) {
        for (int i = 0; i < free_.size() - 1; i++) {
            std::pair<size_t, size_t> tmp = free_[i];
            if (tmp.first == index + n) {
                free_.back().second += tmp.second;
                free_.erase(free_.begin() + i);
            }
            if (tmp.first + tmp.second == index) {
                free_.back().first = free_[i].first;
                free_.back().second += free_[i].second;
                free_.erase(free_.begin() + i);
            }
        }
    }

    ~Pool() {
        operator delete(start_, size_ * sizeof(T));
    }

};

template<typename T, typename U>
bool operator==(const Pool<T>& lhs, const Pool<U>& rhs) {
    if (lhs.free().size() != rhs.free().size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.free().size(); ++i) {
        if (lhs.free()[i] != rhs.free()[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, typename U>
bool operator!=(const Pool<T>& lhs, const Pool<U>& rhs) {
    return !(lhs == rhs);
}