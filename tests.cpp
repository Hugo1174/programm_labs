#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>

// Копируем сюда функцию split (или выносим в .h)
std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}

// ТЕСТ 1: Проверка функции разделения строки
TEST(IPFilterTest, SplitFunction) {
    std::vector<std::string> expected = {"192", "168", "0", "1"};
    ASSERT_EQ(split("192.168.0.1", '.'), expected);
    
    std::vector<std::string> expected_empty = {""};
    ASSERT_EQ(split("", '.'), expected_empty);
}

// ТЕСТ 2: Проверка лексикографической сортировки (обратной)
TEST(IPFilterTest, ReverseSort) {
    std::vector<std::vector<int>> ip_pool = {
        {1, 1, 1, 1},
        {1, 2, 1, 1},
        {1, 10, 1, 1}
    };
    
    std::sort(ip_pool.begin(), ip_pool.end(), [](const std::vector<int> &a, const std::vector<int> &b) {
        return a > b; 
    });

    std::vector<std::vector<int>> expected = {
        {1, 10, 1, 1},
        {1, 2, 1, 1},
        {1, 1, 1, 1}
    };

    ASSERT_EQ(ip_pool, expected);
}

// ТЕСТ 3: Проверка фильтрации (любой байт равен 46)
TEST(IPFilterTest, FilterAny46) {
    std::vector<std::vector<int>> ip_pool = {
        {46, 70, 2, 1},
        {1, 2, 46, 4},
        {1, 2, 3, 4}
    };
    
    std::vector<std::vector<int>> result;
    for (const auto &ip : ip_pool) {
        if (std::any_of(ip.begin(), ip.end(), [](int b) { return b == 46; })) {
            result.push_back(ip);
        }
    }

    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0][0], 46);
    ASSERT_EQ(result[1][2], 46);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}