#include "lib/lib.h"

#include <gtest/gtest.h>

#include <vector>
#include <list>

TEST(FunctionTests, all_of) {
    std::vector<int> a = {1, 3, 5, 7};
    ASSERT_TRUE(algorithms::all_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    a[0] = 9;
    ASSERT_TRUE(algorithms::all_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    a.push_back(6);
    ASSERT_FALSE(algorithms::all_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_FALSE(algorithms::all_of(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));

    std::list<std::string> b = {"abc", "bca", "rty", "ytr"};
    ASSERT_TRUE(algorithms::all_of(b.begin(), b.end(), [](std::string i) { return i.size() == 3; }));
    b.push_back("tyr");
    ASSERT_TRUE(algorithms::all_of(b.begin(), b.end(), [](std::string i) { return i.size() == 3; }));
    b.push_front("qwer");
    ASSERT_FALSE(algorithms::all_of(b.begin(), b.end(), [](std::string i) { return i.size() == 3; }));
}

TEST(FunctionTests, any_of) {
    std::vector<int> a = {1, 3, 5, 7};
    ASSERT_TRUE(algorithms::any_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_FALSE(algorithms::any_of(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));
    a[0] = 9;
    ASSERT_FALSE(algorithms::any_of(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));
    a.push_back(6);
    ASSERT_FALSE(algorithms::all_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_TRUE(algorithms::any_of(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));

    std::list<std::string> b = {"abc", "bca", "rty", "ytr"};
    ASSERT_FALSE(algorithms::any_of(b.begin(), b.end(), [](std::string i) { return i.size() > 3; }));
    b.push_back("tyrr");
    ASSERT_TRUE(algorithms::any_of(b.begin(), b.end(), [](std::string i) { return i.size() > 3; }));
    b.push_front("qwer");
    ASSERT_FALSE(algorithms::any_of(b.begin(), b.end(), [](std::string i) { return i.size() < 3; }));
}

TEST(FunctionTests, none_of) {
    std::vector<int> a = {1, 3, 5, 7};
    ASSERT_TRUE(algorithms::none_of(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));
    ASSERT_FALSE(algorithms::none_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    a[0] = 9;
    ASSERT_FALSE(algorithms::none_of(a.begin(), a.end(), [](int i) { return i % 5 == 0; }));
    a.push_back(6);
    ASSERT_FALSE(algorithms::none_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_TRUE(algorithms::none_of(a.begin(), a.end(), [](int i) { return i > 100; }));

    std::list<std::string> b = {"abc", "bca", "rty", "ytr"};
    ASSERT_TRUE(algorithms::none_of(b.begin(), b.end(), [](std::string i) { return i.size() > 3; }));
    b.push_back("tyrr");
    ASSERT_TRUE(algorithms::none_of(b.begin(), b.end(), [](std::string i) { return i.size() < 3; }));
    b.push_front("qwaer");
    ASSERT_FALSE(algorithms::none_of(b.begin(), b.end(), [](std::string i) { return i.size() == 5; }));
}

TEST(FunctionTests, one_of) {
    std::vector<int> a = {1, 3, 5, 7};
    ASSERT_TRUE(algorithms::one_of(a.begin(), a.end(), [](int i) { return i == 5; }));
    ASSERT_FALSE(algorithms::one_of(a.begin(), a.end(), [](int i) { return i % 2 == 1; }));
    a[0] = 5;
    ASSERT_FALSE(algorithms::one_of(a.begin(), a.end(), [](int i) { return i == 5; }));

    std::list<std::string> b = {"abc", "bca", "rty", "yutr"};
    ASSERT_TRUE(algorithms::one_of(b.begin(), b.end(), [](std::string i) { return i.size() == 4; }));
    b.push_back("tyrr");
    ASSERT_FALSE(algorithms::one_of(b.begin(), b.end(), [](std::string i) { return i.size() == 4; }));
    b.push_front("er");
    ASSERT_TRUE(algorithms::one_of(b.begin(), b.end(), [](std::string i) { return i.size() == 2; }));
}

TEST(FunctionTests, is_partinated) {
    std::vector<int> a {1, 2, 3, 4, 5, 6, 7};
    ASSERT_FALSE(algorithms::is_partitioned(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));
    std::partition(a.begin(), a.end(), [](int i) { return i % 2 == 0; });
    ASSERT_TRUE(algorithms::is_partitioned(a.begin(), a.end(), [](int i) { return i % 2 == 0; }));

    std::list<int> b{1, 2, 3, 4, 5, 6, 7};
    ASSERT_FALSE(algorithms::is_partitioned(b.begin(), b.end(), [](int i) { return i % 2 == 0; }));
    std::partition(b.begin(), b.end(), [](int i) { return i % 2 == 0; });
    ASSERT_TRUE(algorithms::is_partitioned(b.begin(), b.end(), [](int i) { return i % 2 == 0; }));
}

TEST(FunctionTests, find_not) {
    std::vector<int> a{7, 7, 2, 7, 5, 7, 7};
    ASSERT_EQ(algorithms::find_not(a.begin(), a.end(), 7), a.begin() + 2);
    ASSERT_EQ(algorithms::find_not(a.begin(), a.end(), 0), a.begin());

    std::list<int> b{7, 7, 2, 7, 5, 7, 7};
    ASSERT_EQ(*(algorithms::find_not(b.begin(), b.end(), 7)), 2);
    ASSERT_EQ(algorithms::find_not(b.begin(), b.end(), 0), b.begin());

    std::vector<int> c{7, 7, 7, 7, 7, 7, 7};
    ASSERT_EQ(algorithms::find_not(c.begin(), c.end(), 7), c.end());
}

TEST(FunctionTests, find_backward) { // ? - list
    std::vector<int> a{7, 7, 2, 7, 5, 7, 7};
    ASSERT_EQ(algorithms::find_backward(a.begin(), a.end(), 5), a.begin() + 4);
    ASSERT_EQ(algorithms::find_backward(a.begin(), a.end(), 2), a.begin() + 2);
    ASSERT_EQ(algorithms::find_backward(a.begin(), a.end(), 0), a.end());
}

TEST(FunctionTests, is_sorted) {
    std::vector<int> a{1, 3, 5, 7};
    ASSERT_TRUE(algorithms::is_sorted(a.begin(), a.end()));
    ASSERT_FALSE(algorithms::is_sorted(a.begin(),a.end(),[](int a,int b){return a > b;}));
    std::vector<int> b{8, 6, 4, 2};
    ASSERT_TRUE(algorithms::is_sorted(b.begin(),b.end(),[](int a,int b){return a > b;}));
    ASSERT_FALSE(algorithms::is_sorted(b.begin(),b.end()));

    std::list<int> c{1, 3, 5, 7};
    ASSERT_TRUE(algorithms::is_sorted(c.begin(), c.end()));
    ASSERT_FALSE(algorithms::is_sorted(c.begin(), c.end(),[](int a,int b){return a > b;}));
    std::list<int> d{8, 6, 4, 2};
    ASSERT_TRUE(algorithms::is_sorted(d.begin(), d.end(),[](int a,int b){return a > b;}));
    ASSERT_FALSE(algorithms::is_sorted(d.begin(), d.end()));
}

TEST(FunctionTests, pallindrome_vector) {
   std::vector<int> a = {1, 2, 3, 2, 1};

    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), true);
    a[2] = 9;
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), true);

    a.push_back(1);
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), false);
    a[2] = 3;
    a[3] = 3;
    a[4] = 2;
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), true);
}

TEST(FunctionTests, pallindrome_list) {
    std::list<int> a = {1, 2, 3, 2, 1};

    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), true);
    a.push_back(0);
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), false);

    a.push_front(0);
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end()), true);
}

TEST(FunctionTests, pallindrome_vector_predicate) {
    std::vector<int> a = {4, 2, 3, 8, 1};

    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end(), [](int i) { return i % 3; }), true);
    a[2] = 11;
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end(), [](int i) { return i % 3; }), true);

    a.push_back(1);
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end(), [](int i) { return i % 3; }), false);
    //a[2] = 3
    a[2] = 15;
    a[3] = 6;
    a[4] = 5;
    a[5] = 10;
    ASSERT_EQ(algorithms::is_palindrome(a.begin(), a.end(), [](int i) { return i % 3; }), true);
}

TEST(FunctionTests, count) {
    std::vector<int> a = {4, 2, 3, 2, 1};

    ASSERT_EQ(algorithms::count(a.begin(), a.end(), 2), 2);
    a[0] = 2;
    ASSERT_EQ(algorithms::count(a.begin(), a.end(), 2), 3);

    std::list<int> b = {1, 2, 3, 2, 1};

    ASSERT_EQ(algorithms::count(b.begin(), b.end(), 2), 2);
    b.push_back(2);
    ASSERT_EQ(algorithms::count(b.begin(), b.end(), 2), 3);
    b.push_back(8);
    ASSERT_EQ(algorithms::count(b.begin(), b.end(), 2), 3);
}

TEST(FunctionTests, count_if) {
    std::vector<int> a = {4, 9, 3, 15, 1};

    ASSERT_EQ(algorithms::count_if(a.begin(), a.end(), [](int i) { return i > 5; }), 2);
    a[0] = 9;
    ASSERT_EQ(algorithms::count_if(a.begin(), a.end(), [](int i) { return i > 5; }), 3);

    std::list<int> b = {14, 5, 34, 2, 15};

    ASSERT_EQ(algorithms::count_if(b.begin(), b.end(), [](int i) { return i % 5 == 0; }), 2);
    b.push_back(20);
    ASSERT_EQ(algorithms::count_if(b.begin(), b.end(), [](int i) { return i % 5 == 0; }), 3);
    b.push_back(9);
    ASSERT_EQ(algorithms::count_if(b.begin(), b.end(), [](int i) { return i % 5 == 0; }), 3);
}

TEST(FunctionTests, for_each) {
    std::vector<int> a{3, -4, -1, -8, -1, 267, -1, 0, 0, 33, 12};
    ASSERT_EQ(algorithms::count_if(a.begin(), a.end(), [](int i) { return i < 0; }), 5);
    algorithms::for_each(a.begin(), a.end(), [](int &n) { n++; });
    ASSERT_EQ(algorithms::count_if(a.begin(), a.end(), [](int i) { return i < 0; }), 2);

    int sum = 0;
    for (auto v : a) {
        sum += v;
    }
    algorithms::for_each(a.begin(), a.end(), [](int &n) { n *= 5; });
    int sum2 = 0;
    for (auto v : a) {
        sum2 += v;
    }
    ASSERT_EQ(sum * 5, sum2);
}

TEST(XrangeTests, constructorTest) {
    auto x = xrange(1, 10);
    std::list<int> a{x.begin(), x.end()};
    std::vector<int> b{x.begin(), x.end()};
    for (int i = 0; i < b.size(); i++) {
        ASSERT_EQ(b[i], i + 1);
    }

    auto d = xrange(1.5, 5.5);
    std::vector<double> v{d.begin(), d.end()};
    double k = 1.5;
    for (int i = 0; i < v.size(); i++) {
        ASSERT_EQ(v[i], k);
        k++;
    }
}

TEST(XrangeTests, forTest) {
    int k = 1;
    for (auto i : xrange(1, 10, 2)) {
        ASSERT_EQ(k, i);
        k += 2;
    }
}

TEST(XrangeTests, minusforTest1) {
    int k = 10;
    for (auto i : xrange(10, 1, -1)) {
        ASSERT_EQ(k, i);
        k--;
    }
}

TEST(XrangeTests, minusforTest2) {
    int k = -10;
    for (auto i : xrange(-10, -20, -1)) {
        ASSERT_EQ(k, i);
        k--;
    }
}

TEST(ZipTests, differenceTest) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    auto a = zip(l, v);
    auto b = zip(v, l);
    std::vector<std::pair<int, char>> z = {a.begin(), a.end()};
    std::vector<std::pair<int, char>> q = {b.begin(), b.end()};
    for(int i = 0; i < 4; i++) {
        ASSERT_FALSE(z[i].first == q[i].first);
        ASSERT_FALSE(z[i].second == q[i].second);
        ASSERT_TRUE(z[i].first == q[i].second);
        ASSERT_TRUE(z[i].second == q[i].first);
    }
}

TEST(ZipTests, constructorTest) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    auto a = zip(l, v);
    std::list<int>* l_a = &l;
    std::vector<std::pair<int, char>> z = {a.begin(), a.end()};

    std::list<int>::iterator r = l.begin();
    for(int i = 0; i < 4; i++) {
        ASSERT_EQ(z[i].first, *r);
        r++;
        ASSERT_EQ(z[i].second, v[i]);
    }
}

TEST(ZipTests, forTest) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};

    int t = 0;
    std::list<int>::iterator r = l.begin();
    for(auto value : zip(l, v)) {
        ASSERT_EQ(value.first, *r);
        r++;
        ASSERT_EQ(value.second, v[t]);
        t++;
    }
}

TEST(ProductTests, constructTest) {
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};

    auto a = product(l, v);
    std::vector<std::pair<int, char>> z = {a.begin(), a.end()};
    int t = 0;
    for (auto value : z) {
        //std::cout << value.first << " " << value.second << std::endl;
        ASSERT_EQ(v[t % 4], value.second);
        t++;
    }
    ASSERT_EQ(z.size(), 20);
}

TEST(ProductTests, productOneString) {
    std::list<int> l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    std::string e = "abcd";
    auto a = product_one(e, 3);
    //auto c = {a.begin(), a.end()};
    std::vector<std::string> z = {a.begin(), a.end()};
    //std::cout << z.size()<< '\n';
    char old = 'a';
//    for (size_t i = 0; i < z.size(); i++) {
//        for (size_t j = 0; j < z[i].size(); j++) {
//            std::cout << z[i][j] << " ";
//        }
//        std::cout << '\n';
//    }
    for (size_t i = 0; i < z.size(); i++) {
        ASSERT_TRUE(old <= z[i][0]);
        old = z[i][0];
    }
}

TEST(ProductTests, productOne) {
    std::list<int> l = {1, 2, 3};
    std::vector v = {'a', 'b', 'c', 'd'};
    std::string e = "abcd";
    auto a = product_one(v, 3);
    std::vector<std::vector<char>> z = {a.begin(), a.end()};
    ASSERT_EQ(z.size(), 64);
    for (size_t i = 0; i < z.size(); i++) {
        for (size_t j = 0; j < z[i].size(); j++) {
            //std::cout << z[i][j] << " ";
            if (i == 63) {
                ASSERT_EQ(z[i][j], 'd');
            }
        }
        //std::cout << '\n';
    }

    auto b = product_one(l, 3);
    std::vector<std::list<int>> zz = {b.begin(), b.end()};
    ASSERT_EQ(zz.size(), 27);

    int s1 = 0, s2 = 0, s3 = 0;
    for (auto res : product_one(l, 3)) {
        std::list<int>::iterator it = res.begin();
        s1 += *(res.begin());
        it++;
        s2 += *(it);
        it++;
        s3 += *(it);
    }
    ASSERT_EQ(s1, s2);
    ASSERT_EQ(s2, s3);
}

TEST(ProductTests, EGE) {
    /*
     Все четырёхбуквенные слова, в составе которых могут быть только буквы Л, Е, М, У, Р, записаны в алфавитном порядке и пронумерованы, начиная с 1.
Ниже приведено начало списка.
1. ЕЕЕЕ
2. ЕЕЕЛ
3. ЕЕЕМ
4. ЕЕЕР
5. ЕЕЕУ
6. ЕЕЛЕ
…
Под каким номером в списке идёт первое слово, которое начинается с буквы Л? Источник: демоверсия 2022
     *Answer: 126
     */
    const int ans = 126;
    int k = 0;
    std::vector<char> a = {'E', 'L', 'M', 'R', 'U'};
    for (auto t : product_one(a, 4)) {
        k++;
        if (t[0] == 'L') {
            break;
        }
    }
    ASSERT_EQ(k, ans);
}

template <typename T>
bool greater(const T& a, const T& b) {
    return a > b;
}

TEST(PermutationTests, easy) {
    std::vector<char> a = {'a', 'b', 'c', 'd'};
    //std::function<bool(int, int)> f2 = [](int x, int y) { return x > y; };
    int counter = 0;
    std::vector<std::vector<char>> ans1, ans2;
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }*/
        //std::cout << '\n';
        ans1.emplace_back(a);
        counter++;
    } while (nextPermutation(a.begin(), a.end()));
    ASSERT_EQ(counter, 24);

    a = {'a', 'b', 'c', 'd'};
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }
        std::cout << '\n';*/
        ans2.emplace_back(a);
        counter++;
    } while (std::next_permutation(a.begin(), a.end()));

    ASSERT_EQ(ans1, ans2);
}

TEST(PermutationTests, middle) {
    std::vector<int> a = {4, 3, 2, 1};
    //std::function<bool(int, int)> f2 = [](int x, int y) { return x > y; };
    int counter = 0;
    std::vector<std::vector<int>> ans1, ans2;
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }*/
        //std::cout << '\n';
        ans1.emplace_back(a);
        counter++;
    } while (nextPermutation(a.begin(), a.end()));
    ASSERT_EQ(counter, 1);

    a = {4, 3, 2, 1};
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }
        std::cout << '\n';*/
        ans2.emplace_back(a);
        counter++;
    } while (std::next_permutation(a.begin(), a.end()));

    ASSERT_EQ(ans1, ans2);
}

TEST(PermutationTests, hard) {
    std::vector<int> a = {4, 3, 2, 1};
    //std::function<bool(int, int)> f2 = [](int x, int y) { return x > y; };
    int counter = 0;
    std::vector<std::vector<int>> ans1, ans2;
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }*/
        //std::cout << '\n';
        ans1.emplace_back(a);
        counter++;
    } while (nextPermutation(a.begin(), a.end(), &greater<int>));
    ASSERT_EQ(counter, 24);

    a = {4, 3, 2, 1};
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }
        std::cout << '\n';*/
        ans2.emplace_back(a);
        counter++;
    } while (std::next_permutation(a.begin(), a.end(), &greater<int>));

    ASSERT_EQ(ans1, ans2);

    std::list<int> b = {5, 4, 3, 2, 1};
    counter = 0;
    do {
        counter++;
    } while (nextPermutation(b.begin(), b.end(), &greater<int>));
    ASSERT_EQ(counter, 120);
}

TEST(MultiPermutationTests, easy) {
    std::vector<char> a = {'a', 'b', 'b', 'c', 'd'};
    //std::function<bool(int, int)> f2 = [](int x, int y) { return x > y; };
    int counter = 0;
    int ans1, ans2;
    do {
        /*for (size_t i = 0; i < a.size(); i++) {
            std::cout << a[i] << " ";
        }
        std::cout << '\n';*/
        counter++;
    } while (nextMultiPermutation(a.begin(), a.end()));
    ASSERT_EQ(counter, 60);
    ans1 = counter;

    std::list<char> b = {'a', 'b', 'b', 'c', 'd'};
    counter = 0;
    do {
        counter++;
    } while (nextMultiPermutation(b.begin(), b.end()));
    ASSERT_EQ(counter, ans1);
}

TEST(PermutationTests, generator1) {
    std::vector<char> a = {'a', 'b', 'b', 'c', 'd'};
    auto g = permutations(a);
    int counter = 0;
    for (auto i  = g.begin(); i != g.end(); i++) {
        /*for (size_t j = 0; j < (*i).size(); j++) {
            std::cout << (*i)[j] << " ";
        }*/
        counter++;
        //std::cout << '\n';
    }
    ASSERT_EQ(counter, 60);
    counter = 0;
    for (auto t : g) {
        counter++;
        //std::cout << t.size() << '\n';
    }
    ASSERT_EQ(counter, 60);
    std::list<std::vector<char>> r = {g.begin(), g.end()};
    ASSERT_EQ(r.size(), 60);
}

TEST(PermutationTests, generator2) {
    std::vector<char> a = {'a', 'b', 'c', 'd', 'e'};
    auto g = permutations(a);
    int counter = 0;
    for (auto i  = g.begin(); i != g.end(); i++) {
        /*for (size_t j = 0; j < (*i).size(); j++) {
            std::cout << (*i)[j] << " ";
        }*/
        counter++;
        //std::cout << '\n';
    }
    ASSERT_EQ(counter, 120);
    counter = 0;
    for (auto t : g) {
        counter++;
        //std::cout << t.size() << '\n';
    }
    ASSERT_EQ(counter, 120);
    std::list<std::vector<char>> r = {g.begin(), g.end()};
    ASSERT_EQ(r.size(), 120);
}

TEST(PermutationTests, generator3) {
    //std::vector<char> a = {'a', 'b', 'c', 'd', 'e'};
    auto g = permutations<std::vector<int>>(1, 2, 3, 4, 5);
    //auto g = permutations<std::vector<int>>(1, 2, 3, 4, 5);
    int counter = 0;
    for (auto i  = g.begin(); i != g.end(); i++) {
        for (size_t j = 0; j < (*i).size(); j++) {
            std::cout << (*i)[j] << " ";
        }
        counter++;
        std::cout << '\n';
    }
    ASSERT_EQ(counter, 120);
    counter = 0;

}