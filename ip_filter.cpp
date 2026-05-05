#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Функция разбиения строки
std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}

// Удобный вывод IP-адреса
void print_ip(const std::vector<int>& ip) {
    for (size_t i = 0; i < ip.size(); ++i) {
        std::cout << ip[i] << (i == ip.size() - 1 ? "" : ".");
    }
    std::cout << std::endl;
}

int main() {
    try {
        std::vector<std::vector<int>> ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            if (line.empty()) continue;
            auto v = split(line, '\t');
            auto parts = split(v.at(0), '.');
            
            std::vector<int> ip;
            for (const auto& part : parts) {
                ip.push_back(std::stoi(part));
            }
            ip_pool.push_back(ip);
        }

        // 1. Обратная лексикографическая сортировка
        std::sort(ip_pool.begin(), ip_pool.end(), [](const auto& a, const auto& b) {
            return a > b; // Оператор > для векторов сравнивает элементы по порядку
        });

        // Печать всего списка
        for (const auto& ip : ip_pool) print_ip(ip);

        // 2. Фильтр: первый байт == 1
        for (const auto& ip : ip_pool) {
            if (ip[0] == 1) print_ip(ip);
        }

        // 3. Фильтр: первый == 46, второй == 70
        for (const auto& ip : ip_pool) {
            if (ip[0] == 46 && ip[1] == 70) print_ip(ip);
        }

        // 4. Фильтр: любой байт == 46
        for (const auto& ip : ip_pool) {
            if (std::any_of(ip.begin(), ip.end(), [](int byte) { return byte == 46; })) {
                print_ip(ip);
            }
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}