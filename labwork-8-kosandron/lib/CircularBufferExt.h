#pragma once

#include "../lib/CircularBuffer.h"
template<typename T, typename Allocator = std::allocator_traits<T>>
class CCircularBufferExt : public CCircularBuffer<T, Allocator> {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using iterator = Iterator<T>;
    using const_iterator = ConstIterator<T>;
    using allocator_traits = std::allocator_traits<Allocator>;
    using CCircularBuffer<T, Allocator>::CCircularBuffer;

    void push_back(const_reference value) {
        if (this->size_ == this->capacity_) {
            if (this->capacity_ == 0) {
                this->reserve(1);
            } else {
                this->reserve(this->capacity_ * 2);
            }
        }
        allocator_traits::construct(this->allocator_, this->start_ + this->tail_, value);
        this->tail_ = (this->tail_ + 1) % this->capacity_;
        this->size_++;
    }

    void push_front(const_reference value) {
        if (this->size_ == this->capacity_) {
            if (this->capacity_ == 0) {
                this->reserve(1);
            } else {
                this->reserve(this->capacity_ * 2);
            }
        }
        this->head_ = (this->head_ + this->capacity_ - 1) % this->capacity_;
        allocator_traits::construct(this->allocator_, this->start_ + this->head_, value);
        this->size_++;
    }

    iterator insert(const_iterator e, const_reference value) {
        //auto ans = this->begin();
        iterator it = (e - this->cbegin()) + this->begin();
        if (this->size_ == 0) {
            return it;
        }
        this->push_back(*(this->end() - 1));

        it = this->end() - 1;
        for (; it != (e - this->cbegin()) + this->begin(); it--) {
            *it = *(it - 1);
        }
        *((e - this->cbegin()) + this->begin()) = value;

        return it;
    }

    iterator insert(const_iterator it, size_t n, const T& value) {
        if (this->empty() || n == 0) {
            return ((it - this->cbegin()) + this->begin());
        }
        for (size_t i = 0; i < n; i++) {
            it = ((insert(it, value) - this->begin()) + this->cbegin());
        }

        return ((it - this->cbegin()) + this->begin());
    }

    iterator insert(const_iterator it, std::initializer_list<value_type> list) {
        if (this->empty() || list.size() == 0) {
            return ((it - this->cbegin()) + this->begin());
        }
        for (auto i = list.begin(); i != list.end(); i++) {
            it = ((insert(it, *i) - this->begin()) + this->cbegin() + 1);
        }

        return ((it - this->cbegin()) + this->begin() - list.size());
    }
};