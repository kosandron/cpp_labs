#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T>
class Iterator {
 public:
    using value_type = T;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using size_type = std::size_t;

 private:
    pointer start;
    size_type head = 0;
    size_type tail = 0;
    int pos_ = 0;
    size_type size = 0;
    size_type capacity = 0;
    bool beg = false;
    bool end = false;

 public:
    Iterator(pointer start, size_type pos, size_type head, size_type tail, size_type size, size_type capacity, bool beg_,bool end_) {
        this->start = start;
        pos_ = pos;
        this->head = head;
        this->tail = tail;
        this->size = size;
        this->capacity = capacity;
        beg = beg_;
        end = end_;
    }

    pointer operator->() {
        return *(start[(pos_ + head + capacity) % capacity]);
    }

    reference operator*() {
        return start[(pos_ + head + capacity) % capacity];
    }

    reference operator[](difference_type value){
        return start[(pos_ + head + value) % capacity];
    }

    Iterator& operator--() {
        pos_--;
        beg = (pos_ == 0);
        end = (pos_ == size);

        return *this;
    }

    Iterator& operator--(int) {
        return --(*this);
    }

    Iterator& operator++() {
        ++pos_;
        beg = (pos_ == 0);
        end = (pos_ == size);

        return *this;
    }

    Iterator& operator++(int) {
        return ++(*this);
    }

    Iterator operator+(difference_type value) {
        Iterator<T> ans = *this;
        ans.pos_ = this->pos_ + value;
        ans.beg = (ans.pos_ == 0);
        ans.end = (ans.pos_ == size);

        return ans;
    }

    Iterator operator-(difference_type value){
        Iterator<T> ans = *this;
        ans.pos_ = this->pos_ - value;
        ans.beg = (ans.pos_ == 0);
        ans.end = (ans.pos_ == size);

        return ans;
    }

    Iterator& operator+=(difference_type value) {
        pos_ = pos_ + value;
        beg = (pos_ == 0);
        end = (pos_ == size);

        return *this;
    }

    Iterator& operator-=(difference_type value) {
        pos_ = pos_ - value;
        beg = (pos_ == 0);
        end = (pos_ == size);

        return *this;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            if (lhs.capacity == 0 || rhs.capacity == 0) {
                return (lhs.capacity == rhs.capacity);
            }
            if (lhs.size == 0 && rhs.size == 0) {
                return true;
            }

            return ((lhs.pos_ + lhs.head + lhs.capacity) % lhs.capacity == (rhs.pos_ + rhs.head + rhs.capacity) % rhs.capacity) && (lhs.end == rhs.end) && (lhs.beg == rhs.beg);
    }

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
        return lhs.pos_ < rhs.pos_;
    }

    friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs < rhs);
    }

    friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
        return lhs.pos_ > rhs.pos_;
    }

    friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs < rhs);
    }

    friend Iterator operator+(difference_type value, const Iterator<T>& tmp1) {
        Iterator<T> tmp2 = tmp1;
        tmp2 += value;
        return tmp2;
    }

    friend Iterator operator-(difference_type value, const Iterator& tmp1) {
        Iterator<T> tmp2 = tmp1;
        tmp2 -= value;
        return tmp2;
    }

    friend difference_type operator+(const Iterator& lhs, const Iterator& rhs) {
        return static_cast<difference_type>(lhs.pos_ + rhs.pos_);
    }

    friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
        return static_cast<difference_type>(lhs.pos_ - rhs.pos_);
    }
};
