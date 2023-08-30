#include "../lib/CircularBufferExt.h"

#include <algorithm>
#include <gtest/gtest.h>

TEST(Buffer, clear) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.pop_back();
    b.pop_back();
    ASSERT_TRUE(b.empty());
}

TEST(Buffer, push) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);

    ASSERT_EQ(3, b[0]);
    ASSERT_EQ(4, b[1]);
}

TEST(Buffer, Push_overflow) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_EQ(4, b[0]);
    ASSERT_EQ(5, b[1]);
}

TEST(BufferExt, push) {
    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);

    ASSERT_EQ(3, b[0]);
    ASSERT_EQ(4, b[1]);
}

TEST(BufferExt, Push_overflow) {
    CCircularBufferExt<int, std::allocator<int>> b(1);
    b.push_back(3);
    ASSERT_EQ(b.capacity(), 1);
    ASSERT_EQ(b.size(), 1);
    b.push_back(4);

    ASSERT_EQ(b.capacity(), 2);
    ASSERT_EQ(3, b[0]);
    ASSERT_EQ(4, b[1]);
    ASSERT_EQ(b.capacity(), 2);
}

TEST(Buffer, Push_front) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_front(3);
    b.push_front(4);

    ASSERT_EQ(4, b[0]);
    ASSERT_EQ(3, b[1]);
}

TEST(Buffer, Push_front_overflow) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_front(3);
    b.push_front(4);
    b.push_front(5);

    ASSERT_EQ(5, b[0]);
    ASSERT_EQ(4, b[1]);
}

TEST(BufferExt, Push_front) {
    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_front(3);
    b.push_front(4);

    ASSERT_EQ(4, b[0]);
    ASSERT_EQ(3, b[1]);
}

TEST(BufferExt, Push_front_overflow) {
    CCircularBufferExt<int, std::allocator<int>> b(1);
    b.push_front(3);
    b.push_front(4);

    ASSERT_EQ(4, b[0]);
    ASSERT_EQ(3, b[1]);
    ASSERT_EQ(b.capacity(), 2);
}

TEST(Buffer, frontAndback) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    ASSERT_EQ(b.front(), b[0]);
    ASSERT_EQ(b.back(), b[2]);
}

TEST(BufferExt, frontAndback) {
    CCircularBufferExt<int, std::allocator<int>> b(3);
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    ASSERT_EQ(b.front(), b[0]);
    ASSERT_EQ(b.back(), b[2]);
}

TEST(Buffer, Pop_back) {
    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.pop_back();

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b[0], 3);
}

TEST(BufferExt, Pop_back) {
    CCircularBufferExt<int, std::allocator<int>> b(1);
    b.push_back(3);
    b.push_back(4);
    b.pop_back();

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b[0], 3);
}

TEST(Buffer, Pop_before) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    b.pop_before();

    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.capacity(), 3);
    ASSERT_EQ(b[0], 3);
    ASSERT_EQ(b[1], 4);
}

TEST(BufferExt, Pop_before) {
    CCircularBufferExt<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    b.pop_before();

    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.capacity(), 3);
    ASSERT_EQ(b[0], 3);
    ASSERT_EQ(b[1], 4);
}

TEST(Buffer, popfront) {
    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.pop_front();

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b[0], 4);
}

TEST(BufferExt, popfront) {
    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.pop_front();

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b[0], 4);
}

TEST(Buffer, Front) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(7);
    b.push_front(5);
    b.push_back(10);
    b.push_front(11);

    ASSERT_EQ(b[0], 11);
}

TEST(BufferExt, Front) {
    CCircularBufferExt<int, std::allocator<int>> b(4);
    b.push_back(3);
    b.push_back(4);
    b.push_front(5);
    b.push_back(10);
    b.push_back(11);

    ASSERT_EQ(b.capacity(), 8);
    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b[0], 5);
}

TEST(Buffer, Back) {
    CCircularBuffer<int, std::allocator<int>> b(5);
    b.push_back(5);
    b.push_front(6);
    b.push_back(7);
    b.push_front(8);
    b.push_front(9);

    ASSERT_EQ(b[4], 7);
}

TEST(BufferExt, Back) {
    CCircularBufferExt<int, std::allocator<int>> b(3);
    b.push_back(5);
    b.push_front(6);
    b.push_back(7);
    b.push_front(8);
    b.push_front(9);

    ASSERT_EQ(b[4], 7);
    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b.capacity(), 6);
}

TEST(Buffer, Equality1) {
    CCircularBuffer<int, std::allocator<int>> a(3);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_TRUE(a == b);
}

TEST(Buffer, Equality2) {
    CCircularBuffer<int, std::allocator<int>> a(3);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);
    CCircularBuffer<int, std::allocator<int>> b = a;

    ASSERT_TRUE(a == b);
}

TEST(BufferExt, Equality1) {
    CCircularBufferExt<int, std::allocator<int>> a(4);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);
    a.push_back(6);
    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    b.push_back(6);
    ASSERT_TRUE(a == b);
}

TEST(BufferExt, Equality2) {
    CCircularBufferExt<int, std::allocator<int>> a(4);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);
    a.push_back(6);
    CCircularBufferExt<int, std::allocator<int>> b = a;

    ASSERT_TRUE(a == b);
}

TEST(Buffer, Not_equeal) {
    CCircularBuffer<int, std::allocator<int>> a(3);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);

    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    ASSERT_FALSE(a != b);
}

TEST(BufferExt, Not_equal) {
    CCircularBufferExt<int, std::allocator<int>> a(4);
    a.push_back(4);
    a.push_front(3);
    a.push_back(5);
    a.push_back(6);

    CCircularBufferExt<int, std::allocator<int>> b(2);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    b.push_back(6);

    ASSERT_FALSE(a != b);
}

TEST(Buffer, Erase1) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    auto it = a.erase(a.cbegin());
    size_t counter = 2;
    counter = 2;
    for (auto i = a.begin(); i != a.end(); i++, ++counter) {
        ASSERT_EQ(*i, counter);
    }

}

TEST(Buffer, cfrontAndCback) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    ASSERT_EQ(b.cfront(), b[0]);
    ASSERT_EQ(b.cback(), b[2]);
}

TEST(BufferExt, cfrontAndCback) {
    CCircularBufferExt<int, std::allocator<int>> b(3);
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);

    ASSERT_EQ(b.cfront(), b[0]);
    ASSERT_EQ(b.cback(), b[2]);
}

TEST(Buffer, Erase2) {
    CCircularBuffer<int, std::allocator<int>> a(6);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    a.push_back(6);
    auto it = a.erase(a.cbegin(), a.cbegin() + 2);
    size_t counter = 3;
    for (auto i = a.begin(); i != a.end(); ++i, ++counter) {
        ASSERT_EQ(*i, counter);
    }

}

TEST(Buffer, Insert1) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_front(0);
    auto it = a.insert(a.cbegin() + 1, 1);
    size_t counter = 0;
    for (auto i = a.begin(); i != a.end(); ++i, counter++) {
        ASSERT_EQ(*i, counter);
    }

    ASSERT_EQ(*it, 1);
    ASSERT_EQ(*(it - 1), 0);
    ASSERT_EQ(*(it + 1), 2);
}

TEST(Buffer, Insert2) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_front(0);
    ASSERT_EQ(*(a.begin() + 1), a[1]);
    auto it = a.insert(a.cbegin() + 1, 3, 7);
    auto i = a.begin() + 1;
    ASSERT_EQ(0, a[0]);
    for (; i != a.begin() + 4; ++i) {
        ASSERT_EQ(7, *i);
    }

    ASSERT_EQ(2, a[4]);

    ASSERT_EQ(*it, 7);
    ASSERT_EQ(*(it + 1), 7);
    ASSERT_EQ(*(it - 1), 0);

}

TEST(Buffer, Insert3) {
    CCircularBuffer<int, std::allocator<int>> a(5);
    a.push_back(3);
    a.push_back(4);
    a.push_back(5);
    auto it = a.insert(a.cbegin(), {1, 2});
    size_t counter = 1;
    for (auto i = a.begin(); i != a.end(); ++i, ++counter) {
        ASSERT_EQ(*i, counter);
    }

    ASSERT_EQ(*it, 1);
    ASSERT_EQ(*(it - 1), 5);
    ASSERT_EQ(*(it + 1), 2);
}

TEST(BufferExt, Insert1) {
    CCircularBufferExt<int, std::allocator<int>> a(4);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_front(0);
    auto it = a.insert(a.cbegin() + 1, 1);
    size_t counter = 0;
    for (auto i = a.begin(); i != a.end(); ++i, counter++) {
        ASSERT_EQ(*i, counter);
    }

    ASSERT_EQ(*it, 1);
    ASSERT_EQ(*(it - 1), 0);
    ASSERT_EQ(*(it + 1), 2);
}

TEST(BufferExt, Insert2) {
    CCircularBufferExt<int, std::allocator<int>> a(5);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_front(0);
    auto it = a.insert(a.cbegin() + 1, 3, 7);
    auto i = a.begin() + 1;
    ASSERT_EQ(0, a[0]);
    for (; i != a.begin() + 4; ++i) {
        ASSERT_EQ(7, *i);
    }

    size_t counter = 2;
    for (; i != a.end(); ++i, counter++) {
        ASSERT_EQ(counter, *i);
    }

    ASSERT_EQ(*it, 7);
    ASSERT_EQ(*(it + 1), 7);
    ASSERT_EQ(*(it - 1), 0);
}

TEST(BufferExt, Insert3) {
    CCircularBufferExt<int, std::allocator<int>> a(5);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    a.push_front(0);
    auto it = a.insert(a.cbegin() + 1, {1, 2, 3});
    auto i = a.begin();
    size_t counter = 0;
    for (; i != a.begin() + 4; ++i, ++counter) {
        ASSERT_EQ(*i, counter);
    }

    counter = 2;
    for (; i != a.end(); ++i, counter++) {
        ASSERT_EQ(counter, *i);
    }

    ASSERT_EQ(*it, 1);
    ASSERT_EQ(*(it + 1), 2);
    ASSERT_EQ(*(it - 1), 0);
}

TEST(Buffer, Asign1) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(777);
    a.push_back(666);
    a.push_back(555);
    a.push_back(444);
    a.assign(2, 1);

    for (auto i = a.begin(); i != a.end(); ++i) {
        ASSERT_EQ(1, *i);
    }
}

TEST(Buffer, Asign2) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(777);
    a.push_back(666);
    a.push_back(555);
    a.push_back(444);

    CCircularBuffer<int, std::allocator<int>> b(2);
    b.push_back(111);
    b.push_back(222);
    a.assign(b.begin(), b.end());
    ASSERT_TRUE(b == a);
}


TEST(Buffer, Asign3) {
    CCircularBuffer<int, std::allocator<int>> a(4);
    a.push_back(777);
    a.push_back(666);
    a.push_back(555);
    a.push_back(444);
    a.assign({1, 2, 3});

    size_t counter = 1;
    for (auto i = a.begin(); i != a.end(); ++i, ++counter) {
        ASSERT_EQ(counter, *i);
    }
}

TEST(Buffer, beginEnd) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_EQ(b.end() - b.begin(), 3);
    b.pop_back();
    ASSERT_EQ(b.end() - b.begin(), 2);
    ASSERT_EQ(b.capacity(), 3);
}

TEST(BufferExt, beginEnd) {
    CCircularBufferExt<int, std::allocator<int>> b;
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_EQ(b.end() - b.begin(), 3);
    b.pop_back();
    ASSERT_EQ(b.end() - b.begin(), 2);
    ASSERT_EQ(b.capacity(), 4);
}

TEST(Buffer, cbeginCend) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_EQ(b.cend() - b.cbegin(), 3);
    b.pop_back();
    ASSERT_EQ(b.cend() - b.cbegin(), 2);
    ASSERT_EQ(b.capacity(), 3);
}

TEST(BufferExt, cbeginCend) {
    CCircularBufferExt<int, std::allocator<int>> b;
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    ASSERT_EQ(b.cend() - b.cbegin(), 3);
    b.pop_back();
    ASSERT_EQ(b.cend() - b.cbegin(), 2);
    ASSERT_EQ(b.capacity(), 4);
}

/*TEST(Buffer, MoveTest) {
    CCircularBuffer<int, std::allocator<int>> b(3);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);

    CCircularBuffer<int, std::allocator<int>>&& a = CCircularBuffer<int, std::allocator<int>>(3);

}*/
