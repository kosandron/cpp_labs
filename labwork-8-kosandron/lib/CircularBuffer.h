#pragma once

#include "Iterator.h"
#include "ConstIterator.h"

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using iterator = Iterator<T>;
    using const_iterator = ConstIterator<T>;
    using allocator_traits = std::allocator_traits<Allocator>;

protected:
    T* start_ = nullptr;
    size_t capacity_;
    size_t size_ = 0;
    size_t head_ = 0;
    size_t tail_ = 0;
    Allocator allocator_;

public:
    CCircularBuffer() {
        capacity_ = 0;
        CCircularBuffer(0);
    }

    explicit CCircularBuffer(size_type capacity, const Allocator& allocator = Allocator()) {
        allocator_ = allocator;
        start_ = allocator_traits::allocate(allocator_, capacity);
        capacity_ = capacity;
    }

    CCircularBuffer(const CCircularBuffer& a) {
        allocator_ = a.allocator_;
        start_ = allocator_traits::allocate(allocator_, a.capacity_);
        capacity_ = a.capacity_;
        size_ = a.size_;
        head_ = a.head_;
        tail_ = a.tail_;
        if (size_ != 0) {
            size_t i = 0;
            do {
                allocator_traits::construct(allocator_, start_ + head_, a[i]);
                head_ = (head_ + 1) % capacity_;
                i++;
            } while (head_ != tail_);
            head_ = a.head_;
        }
    }

    friend bool operator==(CCircularBuffer& lhs, CCircularBuffer& rhs) {
        if (lhs.size_ != rhs.size_ || lhs.capacity_ != rhs.capacity_) {
            return false;
        }
        auto tmp1 = lhs.begin();
        auto tmp2 = rhs.begin();
        for (;tmp1 != lhs.end(); tmp1++, tmp2++) {
            if (*tmp1 != *tmp2) {
                return false;
            }

        }

        return true;
    }

    friend bool operator!=(CCircularBuffer& lhs, CCircularBuffer& rhs) {
        return !(lhs == rhs);
    }

    CCircularBuffer& operator=(const CCircularBuffer& a) {
        if (this != &a) {
            clear();
            allocator_ = a.allocator_;
            if (capacity_ != a.capacity_) {
                allocator_.deallocate(start_, capacity_);
                start_ = allocator_.allocate(a.capacity_);
                capacity_ = a.capacity_;
            }
        size_ = a.size_;
        head_ = a.head_;
        tail_ = a.tail_;
        if (size_ != 0) {
            size_t i = 0;
            do {
                allocator_.construct(start_ + head_, a[i]);
                head_ = (head_ + 1) % capacity_;
                i++;
            } while (head_ != tail_);
            head_ = a.head_;
        }
    }

    return *this;
    }

    CCircularBuffer(CCircularBuffer&& other) noexcept {
        //std::cout << "b";
        //clear();
        allocator_traits::deallocate(allocator_, this->start_, this->capacity_);
        //delete [] start_;
        size_ = other.size_;
        head_ = other.head_;
        tail_ = other.tail_;
        capacity_ = other.capacity_;

        start_ = other.start_;
        other.start_ = nullptr;
        other.size_ = 0;
        other.head_ = 0;
        other.tail_ = 0;
    }

    CCircularBuffer& operator=(CCircularBuffer&& other) noexcept {
        //std::cout << "I";
        if (this != &other) {
            //clear();
            allocator_traits::deallocate(allocator_, this->start_, this->capacity_);
            size_ = other.size_;
            head_ = other.head_;
            tail_ = other.tail_;
            capacity_ = other.capacity_;

            start_ = other.start_;
            other.start_ = nullptr;
            other.size_ = 0;
            other.head_ = 0;
            other.tail_ = 0;
        }
    }

    /*CCircularBuffer& operator=(CCircularBuffer a) {
        if (*this != a) {
            clear();
            allocator_ = a.allocator_;
            if (capacity_ < a.size()) {
                allocator_traits::deallocate(allocator_, start_, capacity_);
                start_ = allocator_traits::allocate(allocator_, a.capacity_);
                capacity_ = a.capacity_;
            }
            size_ = a.size_;
            head_ = a.head_;
            tail_ = a.tail_;
            if (size_ != 0) {
                size_t i = 0;
                do {
                    allocator_traits::construct(allocator_, start_ + head_, a[i]);
                    head_ = (head_ + 1) % capacity_;
                    i++;
                } while (head_ != tail_);
                head_ = a.head_;
            }
        }

        return *this;
    }*/

    ~CCircularBuffer() {
        clear();
        allocator_traits::deallocate(allocator_, this->start_, this->capacity_);
    }

    reference operator[](size_t index) {
        if (index >= size_) {
            std::cerr << "Index is out of range!!!" << '\n';
        }

        return start_[(head_ + index) % capacity_];
    }

    const_reference operator[](size_t index) const {
        if (index >= size_) {
            std::cerr << "Index is out of range!!!" << '\n';
        }

        return start_[(head_ + index) % capacity_];
    }

    reference front() {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }

        return start_[head_];
    }

    const_reference cfront() const {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }

        return start_[head_];
    }

    reference back() {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }

        return start_[(tail_ - 1 + capacity_) % capacity_];
    }

    const_reference cback() const {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }

        return start_[(tail_ - 1 + capacity_) % capacity_];
    }

    void pop_front() {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }
        allocator_traits::destroy(allocator_, start_ + head_);
        head_ = (head_ + 1) % capacity_;
        size_--;
    }

    void pop_back() {
        if(size_ == 0){
            throw std::out_of_range("Empty buffer");
        }
        allocator_traits::destroy(allocator_, start_ + tail_);
        tail_ = (tail_ + capacity_ - 1) % capacity_;
        --size_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            pop_front();
        }
        allocator_traits::construct(allocator_, start_ + tail_, value);
        tail_ = (tail_ + 1) % capacity_;
        size_++;
    }

    void pop_before() {
        if (size_ == 0) {
            std::cerr << "A buffer is empty!" << '\n';
        }
        allocator_traits::destroy(allocator_, start_ + (head_ + capacity_ - 1) % capacity_);
        tail_ = (tail_ + capacity_ - 1) % capacity_;
        --size_;
    }

    void push_front(const T& value) {
        if (size_ == capacity_) {
            pop_before();
        }
        head_ = (head_ + capacity_ - 1) % capacity_;
        allocator_traits::construct(allocator_, start_ + head_, value);
        size_++;
    }

    void clear() {
        while (size_ > 0) {
            pop_front();
        }
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return (size_ == 0);
    }

    iterator begin() {
        return iterator(start_, 0, head_, tail_, size_, capacity_, true, false);
    }

    const_iterator cbegin() {
        return const_iterator(start_, 0, head_, tail_, size_, capacity_, true, false);
    }

    iterator end() {
        return iterator(start_, size_, head_, tail_, size_, capacity_, false, true);
    }

    const_iterator cend() {
        return const_iterator(start_, size_, head_, tail_, size_, capacity_, false, true);
    }

    iterator erase(const_iterator e) {
        if (e == cend()) {
            std::cerr << "Trying to erase element not in buffer!";
        }
        iterator it = (e - cbegin()) + begin();
        for (; it + 1 != end(); it++) {
            *it = *(it + 1);
        }
        size_--;

        return it;
    }

    iterator erase(const_iterator e1, const_iterator e2) {
        if (e2 <= e1) {
            std::cerr << "Second iterator is less then first";
        }
        size_t counter = cend() - e2;
        iterator it = (e2 - cbegin()) + begin();
        for (size_t i = 0; i < counter; i++) {
            *(begin() + (e1 - cbegin()) + i) = *(it + i);
        }
        size_ -= e2 - e1;

        return it;
    }

    iterator insert(const_iterator e, const_reference value) {
        //auto ans = this->begin();
        iterator it = (e - cbegin()) + begin();
        if (size_ == 0) {
            return it;
        }
        if (capacity_ != size_) {
            this->push_back(*(end() - 1));
        }
        it = end() - 1;
        for (; it != (e - cbegin()) + begin(); it--) {
            *it = *(it - 1);
        }
        *((e - cbegin()) + begin()) = value;

        return it;
    }

    iterator insert(const_iterator it, size_t n, const T& value) {
        if (this->empty() || n == 0) {
            return ((it - cbegin()) + begin());
        }
        for (size_t i = 0; i < n; i++) {
            it = ((insert(it, value) - begin()) + cbegin());
        }

        return ((it - cbegin()) + begin());
    }

    iterator insert(const_iterator it, std::initializer_list<value_type> list) {
        if (this->empty() || list.size() == 0) {
            return ((it - cbegin()) + begin());
        }
        for (auto i = list.begin(); i != list.end(); i++) {
            it = ((insert(it, *i) - begin()) + cbegin() + 1);
        }

        return ((it - cbegin()) + begin() - list.size());
    }

    void assign(size_t n, T value) {
        CCircularBuffer<T, std::allocator<T>> tmp(n);
        for (size_t i = 0; i < n; i++) {
            tmp.push_back(value);
        }

        *this = tmp;
    }

    void assign(iterator it1, iterator it2) {
        size_t counter = 0;
        for (iterator i = it1; i != it2; ++i) {
            counter++;
        }

        CCircularBuffer<T, std::allocator<T>> tmp(counter);
        for (iterator i = it1; i != it2; ++i) {
            tmp.push_back(*i);
        }

        *this = tmp;
    }

    void assign(std::initializer_list<value_type> list) {
        size_t counter = 0;
        for (auto i = list.begin(); i != list.end(); ++i) {
            counter++;
        }

        CCircularBuffer<T, std::allocator<T>> tmp(counter);
        for (auto i = list.begin(); i != list.end(); ++i) {
            tmp.push_back(*i);
        }

        *this = tmp;
    }

    void resize(size_t n) {
        size_ = n;
        if (capacity_ = 0) {
            reserve(1);
        } else {
            reserve(capacity_ * 2);
        }
    }

    void reserve(size_t n) {
        CCircularBuffer<T, std::allocator<T>> tmp(n);
        size_t counter = 0;
        for (auto i = this->begin(); i != this->end() && counter < n; ++i, ++counter) {
            tmp.push_back(*i);
        }

        *this = tmp;
    }
};