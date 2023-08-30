#include <stdexcept>
#include <stack>

class cat{
public:
    int age = 1;
    double mass = 1;
};

namespace algorithms {
    template<class InputIt, class UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first) {
            if (!p(*first))
                return false;
        }

        return true;
    }

    template<class InputIt, class UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first) {
            if (p(*first)) return true;
        }

        return false;
    }

    template<class InputIt, class UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first) {
            if (p(*first)) return false;
        }

        return true;
    }

    template<class InputIt, class UnaryPredicate>
    bool one_of(InputIt first, InputIt last, UnaryPredicate p) {
        size_t counter = 0;
        for (; first != last; ++first) {
            if (p(*first))
                counter++;
            if (counter > 1) {
                return false;
            }
        }

        return (counter == 1);
    }

    template<class InputIt>
    bool is_sorted(InputIt first, InputIt last) {
        InputIt old = first;
        first++;
        for (; first != last; ++first) {
            if (*old > *first) {
                return false;
            }
            old = first;
        }

        return true;
    }

    template<class InputIt, class Compare>
    bool is_sorted(InputIt first, InputIt last, Compare check) {
        InputIt old = first;
        first++;
        for (; first != last; ++first) {
            if (!check(*old, *first)) {
                return false;
            }
            old = first;
        }

        return true;
    }

    template<class InputIt, class T>
    InputIt find_not(InputIt first, InputIt last, const T &element) {
        for (; first != last; ++first) {
            if (*first != element) {
                return first;
            }
        }

        return last;
    }

    template<class InputIt, class UnaryPredicate>
    bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first)
            if (!p(*first))
                break;
        for (; first != last; ++first)
            if (p(*first))
                return false;
        return true;
    }

    template<class InputIt, class T>
    InputIt find_backward(InputIt first, InputIt last, const T &element) {
        std::stack<InputIt> reversed_container;
        for (; first != last; ++first) {
            reversed_container.push(first);
        }

        while (!reversed_container.empty()) {
            if (element == *(reversed_container.top())) {
                return reversed_container.top();
            }
            reversed_container.pop();
        }

        return last;
    }

    template<class InputIt>
    bool is_palindrome(InputIt first, InputIt last) {
        std::stack<InputIt> reversed_container;
        InputIt tmp = first;
        size_t n = 0;
        while (tmp != last) {
            n++;
            tmp++;
        }

        for (size_t i = 0; i < n / 2; i++) {
            reversed_container.push(first);
            first++;
        }

        if (n % 2 == 1) {
            first++;
        }

        while (!reversed_container.empty()) {
            if (*first != *(reversed_container.top())) {
                return false;
            }

            first++;
            reversed_container.pop();
        }

        return true;
    }


    template<class InputIt, class UnaryPredicate>
    bool is_palindrome(InputIt first, InputIt last, UnaryPredicate p) {
        std::stack<InputIt> reversed_container;
        InputIt tmp = first;
        size_t n = 0;
        while (tmp != last) {
            n++;
            tmp++;
        }

        for (size_t i = 0; i < n / 2; i++) {
            reversed_container.push(first);
            first++;
        }

        if (n % 2 == 1) {
            first++;
        }

        while (!reversed_container.empty()) {
            if (p(*first) != p(*(reversed_container.top()))) {
                return false;
            }

            first++;
            reversed_container.pop();
        }

        return true;
    }

    template<class Iterator, class T>
    uint32_t count(Iterator first, Iterator last, const T& value) {
        uint32_t ans = 0;
        for (; first != last; ++first) {
            if (*first == value) {
                ++ans;
            }
        }

        return ans;
    }

    template<class Iterator, class UnaryPredicate>
    uint32_t count_if(Iterator first, Iterator last, UnaryPredicate p) {
        uint32_t ans = 0;
        for (; first != last; ++first) {
            if (p(*first)) {
                ++ans;
            }
        }

        return ans;
    }

    template<class Iterator, class UnaryFunction>
    UnaryFunction for_each(Iterator first, Iterator last, UnaryFunction f) {
        for (; first != last; ++first) {
            f(*first);
        }

        return f;
    }

}

template<class T>
class xrange {
private:
    T start_ = T();
    T stop_;
    size_t step_ = 1;
public:
    class iterator {
    private:
        T current_;
        T step_;
        T stop_;
      public:
            typedef std::input_iterator_tag iterator_category;
            typedef T value_type;
            typedef T difference_type;
            typedef T* pointer;
            typedef T& reference;

            iterator(T current, T stop, T step) : current_(current), stop_(stop), step_(step) {}

            bool operator==(const iterator& other) const {
                return current_ == other.current_;
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }

            reference operator*() {
                return current_;
            }

            iterator& operator++() {
                if (abs(current_) + step_ <= abs(stop_))
                    current_ += step_;
                else
                    current_ = stop_;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }
        };

        xrange(const T start, const T stop, size_t step = 1) : start_(start), stop_(stop), step_(step) {}
        xrange(const T stop, size_t step = 1) : stop_(stop), step_(step) {}

        iterator begin() const {
            return iterator(start_, stop_, step_);
        }

        iterator end() const {
            return iterator(stop_, stop_,  step_);
        }
};
/*
//template<class T>
class xrange2 {
private:
    size_t start_;
    size_t stop_;
    size_t step_ = 1;
public:
    class iterator {
    private:
        cat current_;
        size_t start_;
        size_t step_;
        size_t stop_;
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef cat value_type;
        typedef cat difference_type;
        typedef cat* pointer;
        typedef cat& reference;

        iterator(size_t start, size_t stop, size_t step) : start_(start), stop_(stop), step_(step) {
            if (start == stop) {
                current_.age = start;
                current_.mass = start;
            } else {
                stop = stop + 1;
            }

        }

        bool operator==(const iterator& other) const {
            return (current_.age == other.current_.age && current_.mass == other.current_.mass);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        reference operator*() {
            return current_;
        }

        iterator& operator++() {
            if (abs(current_.age) + step_ <= abs(stop_) || abs(current_.age) == abs(stop_) && current_.mass + step_ <= abs(stop_)) {
                if (current_.mass + step_ <= abs(stop_)) {
                    current_.mass += step_;
                } else {
                    current_.mass = start_;
                    current_.age++;
                }
            } else {
                current_.age = stop_;
                current_.mass = stop_;
            }

            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }
    };

    xrange2(size_t start, size_t stop, size_t step = 1) : start_(start), stop_(stop), step_(step) {}
    //xrange2(size_t stop, size_t step = 1) : stop_(stop), step_(step) {}

    iterator begin() const {
        return iterator(start_, stop_, step_);
    }

    iterator end() const {
        return iterator(stop_, stop_, step_);
    }
};*/

template<typename T, typename U>
class zip {
private:
    T* first_;
    U* second_;
    size_t size_;
public:
    zip(T& first, U& second){
        first_ = &first;
        second_ = &second;
        size_ = std::min(first.size(), second.size());
    }

    class iterator {
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef std::pair<typename T::value_type, typename U::value_type> value_type;
        typedef std::pair<typename T::value_type, typename U::value_type> difference_type;
        typedef std::pair<typename T::value_type, typename U::value_type>* pointer;
        typedef std::pair<typename T::value_type, typename U::value_type>& reference;
    private:
        size_t position_ = 0;
        size_t size_;
        typename T::iterator first_iterator_;
        typename U::iterator second_iterator_;
    public:
        iterator(typename T::iterator first, typename U::iterator second, size_t position, size_t size){
            first_iterator_ = first;
            second_iterator_ = second;
            position_ = position;
            size_ = size;
        }

        bool operator==(const iterator& other) const {
            return position_ == other.position_;
        }

        bool operator!=(const iterator& other) const {
            return position_ != other.position_;
        }

        value_type operator*() {
            return {*first_iterator_, *second_iterator_};
        }

        iterator& operator++() {
            if(position_ + 1 <= size_){
                position_++;
                first_iterator_++;
                second_iterator_++;
            } else {
                position_ = size_;
            }

            return *this;
        }

        iterator operator++(int) {
            ++(*this);
            return *this;
        }
    };

    iterator begin() {
        return iterator((*first_).begin(), (*second_).begin() ,0 ,size_);
    }

    iterator end() {
        return iterator((*first_).end(), (*second_).end(), size_, size_);
    }
};

size_t binpow(size_t a, size_t n) {
    if (n == 0)
        return 1;
    if (n % 2 == 1)
        return binpow (a, n - 1) * a;
    else {
        size_t b = binpow (a, n / 2);
        return b * b;
    }
}

#include <vector>
template<typename T>
class product_one {
private:
    T* array_;
    size_t size_;
    size_t repeat;
public:
    product_one(T& first, size_t repeat){
        array_ = &first;
        size_ = first.size();
        this->repeat = repeat;
    }

    class iterator {
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef std::vector<typename T::value_type> value_type;
        typedef std::vector<typename T::value_type> difference_type;
        typedef std::vector<typename T::value_type>* pointer;
        typedef std::vector<typename T::value_type>& reference;

    private:
        size_t position_ = 0;
        size_t repeat_;
        size_t input_size_;
        size_t count_of_variations_;
        typename T::iterator first_iterator_;
        typename T::iterator first_iterator_begin_;
        typename T::iterator first_iterator_last_;
        std::vector<typename T::iterator> current_iterator_;
    public:
        iterator(typename T::iterator r, typename T::iterator last,  size_t position, size_t input_size, size_t repeat){
            first_iterator_ = r;
            first_iterator_last_ = last;
            first_iterator_begin_ = first_iterator_;
            this->repeat_ = repeat;
            input_size_ = input_size;
            current_iterator_.assign(repeat_, r);
            position_ = position;
            count_of_variations_ = binpow(input_size, repeat);
        }

        bool operator==(const iterator& other) const {
            return position_ == other.position_;
        }

        bool operator!=(const iterator& other) const {
            return position_ != other.position_;
        }

        T operator*() {
            T cur;
            for (size_t i = 0; i < repeat_; i++) {
                cur.push_back(*(current_iterator_[i]));
            }

            return cur;
        }

        iterator& operator++() {
            if(position_ + 1 < count_of_variations_) {
                for (int i = repeat_ - 1 ; i >= 0 ; i--) {
                    current_iterator_[i]++;
                    if (current_iterator_[i] != first_iterator_last_) {
                        position_++;
                        return *this;
                    }

                    current_iterator_[i] = first_iterator_begin_;
                }
            } else {
                position_ = count_of_variations_;
            }

            return *this;
        }

        iterator operator++(int) {
            ++(*this);
            return *this;
        }
    };

    iterator begin() {
        return iterator((*array_).begin(), (*array_).end(), 0, size_, repeat);
    }

    iterator end() {
        return iterator((*array_).end(), (*array_).end(),  binpow(size_, repeat), size_, repeat);
    }
};


template<typename T, typename U>
class product {
private:
    T* first_;
    U* second_;
    size_t size1_, size2_;
public:
    product(T& first, U& second){
        first_ = &first;
        second_ = &second;
        size1_ = first.size();
        size2_ = second.size();
    }

    class iterator {
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef std::pair<typename T::value_type, typename U::value_type> value_type;
        typedef std::pair<typename T::value_type, typename U::value_type> difference_type;
        typedef std::pair<typename T::value_type, typename U::value_type>* pointer;
        typedef std::pair<typename T::value_type, typename U::value_type>& reference;
    private:
        size_t position_ = 0;
        size_t size1_, size2_;
        typename T::iterator first_iterator_;
        typename U::iterator second_iterator_;
        typename U::iterator second_iterator_begin_;
    public:
        iterator(typename T::iterator first, typename U::iterator second, size_t position, size_t size1_, size_t size2_){
            first_iterator_ = first;
            second_iterator_ = second;
            second_iterator_begin_ = second_iterator_;
            position_ = position;
            this->size1_ = size1_;
            this->size2_ = size2_;
        }

        bool operator==(const iterator& other) const {
            return position_ == other.position_;
        }

        bool operator!=(const iterator& other) const {
            return position_ != other.position_;
        }

        value_type operator*() {
            return {*first_iterator_, *second_iterator_};
        }

        iterator& operator++() {
            if(position_ + 1 <= size1_ * size2_){
                position_++;
                if (position_ % size2_ == 0) {
                    first_iterator_++;
                    second_iterator_ = second_iterator_begin_;
                } else {
                    second_iterator_++;
                }
            } else {
                position_ = size1_ * size2_;
            }

            return *this;
        }

        iterator operator++(int) {
            ++(*this);
            return *this;
        }
    };

    iterator begin() {
        return iterator((*first_).begin(), (*second_).begin(), 0 , size1_, size2_);
    }

    iterator end() {
        return iterator((*first_).end(), (*second_).end(), size1_ * size2_, size1_, size2_);
    }
};

//алгоритм Нарайаны
template <typename Iterator, typename T>
bool nextPermutation(const Iterator  begin, const Iterator  end, bool (*compare)(const T , const T)) {
    if (begin == end)
        return false;
    // найти такой наибольший j, для которого a[i] < a[i + 1]
    Iterator i = end, j = end; --i;
    do {
        if (i == begin)
            return false;
        --i;
        --j;
    } while (!(*compare)(*i, *j));

    // увеличить a[i]. Для этого надо найти наибольшее l > i | a[l] > a[i]. Поменять a[i] и a[l]
    Iterator l = end;
    --l;
    while (l != i && !(*compare)(*i, *l)) {
        --l;
    }
    std::iter_swap(i, l);

    // записать последовательность a[i + 1]...a[n] в обратном порядке
    ++i;
    j = end;
    for (; (i != j) && (i != --j); ++i)
        std::iter_swap(i, j);
    return true;
}

template <typename Iterator>
bool nextPermutation(const Iterator begin, const Iterator end) {
    if (begin == end)
        return false;
    // найти такой наибольший j, для которого a[i] < a[i + 1]
    Iterator i = end, j = end; --i;
    do {
        if (i == begin)
            return false;
        --i;
        --j;
    } while (!(*i < *j));

    // увеличить a[i]. Для этого надо найти наибольшее l > i | a[l] > a[i]. Поменять a[i] и a[l]
    Iterator l = end;
    while (l != i && !(*i < *--l));
    std::iter_swap(i, l);

    // записать последовательность a[i + 1]...a[n] в обратном порядке
    ++i;
    j = end;
    for (; (i != j) && (i != --j); ++i)
        std::iter_swap(i, j);
    return true;
}

template <typename Iterator>
bool nextMultiPermutation(const Iterator begin, const Iterator end) {
    if (begin == end)
        return false;
    Iterator i = end, j = end;
    --i; --i; --j;
    while ((i != begin) && (*i >= *j)) {
        --i;
        --j;
    }

    Iterator z = end, g;
    --z;
    if (!(i == begin && *i >= *j)) {
        Iterator l = i;
        ++l;
        Iterator g = l;
        ++g;
        while (l != z && (*i < *(g))) {
            ++l;
            ++g;
        }
        std::iter_swap(i, l);

        ++i;
        j = end;
        --j;
        size_t k = 0;
        size_t i_index, j_index;
        for (Iterator b = begin; b != end; b++) {
            if (b == i) {
                i_index = k;
            }
            if (b == j) {
                j_index = k;
                break;
            }
            k++;
        }
        for (; (i != j) && (i_index < j_index);) {
            std::iter_swap(i, j);
            ++i;
            --j;
            i_index++;
            j_index--;
        }
        return true;
    } else
        return false;
}

template<typename T>
std::vector<T> unpack(const T value) {
    return {value};
}

template<typename T>
std::vector<T> unpack() {
    return {};
}

template<typename T, typename... TArgs>
std::vector<T> unpack(const T value, const TArgs... args) {
    std::vector<T> values;
    values.push_back(value);

    std::vector<T> other_values = unpack(args...);
    values.insert(values.end(), other_values.begin(), other_values.end());

    return values;
}

#include<unordered_set>
template<typename T>
class permutations {
private:
    T* array_;
    bool is_set = true;
public:
    permutations(T& array) {
          array_ = &array;
          std::unordered_set<typename T::value_type> temp;
          for (auto v : array) {
              temp.insert(v);
          }
          is_set = (temp.size() == array.size());
    }

    template<typename U, typename... TArgs>
    permutations<T>(U&& value, TArgs&&... args) {
        static T tempy = unpack(value, args...);
        array_ = &tempy;
        std::unordered_set<typename T::value_type> temp;
        for (auto v : tempy) {
            temp.insert(v);
        }
        is_set = (temp.size() == tempy.size());
    }

    class iterator {
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;
        typedef T difference_type;
        typedef T* pointer;
        typedef T& reference;
    private:
        T array_;
        bool current_flag_ = true;
        bool unical_flag_;
    public:
        iterator(T array, bool current_flag, bool unical_flag)
        : array_(array)
        , current_flag_(current_flag)
        , unical_flag_(unical_flag){}

        bool operator==(const iterator& other) const {
            return current_flag_ == other.current_flag_;
        }

        bool operator!=(const iterator& other) const {
            return current_flag_ != other.current_flag_;
        }

        T operator*() {
            return array_;
        }

        iterator& operator++() {
            if (unical_flag_)
                current_flag_ = nextPermutation(array_.begin(), array_.end());
            else
                current_flag_ = nextMultiPermutation(array_.begin(), array_.end());
            return *this;
        }

        iterator operator++(int) {
            ++(*this);
            return *this;
        }
    };

    iterator begin() {
        return iterator(*array_, true, is_set);
    }

    iterator end() {
        return iterator(*array_, false, is_set);
    }
};
