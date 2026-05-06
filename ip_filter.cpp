#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>


//Разделяет строку на части по указанному символу-разделителю.

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);

    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}


//Печатает IP-адрес в формате n.n.n.n.

void print_ip(const std::vector<int> &ip)
{
    for (auto it = ip.cbegin(); it != ip.cend(); ++it)
    {
        if (it != ip.cbegin()) std::cout << ".";
        std::cout << *it;
    }
    std::cout << "\n";
}

int main()
{
    try
    {
        std::vector<std::vector<int>> ip_pool;

        for (std::string line; std::getline(std::cin, line);)
        {
            if (line.empty()) continue;

            auto v = split(line, '\t');
            if (v.empty()) continue;

            auto parts = split(v.at(0), '.');
            if (parts.size() != 4) continue;

            std::vector<int> ip;
            bool valid = true;
            for (const auto &part : parts)
            {
                // Проверка, что в части адреса только цифры (защита от мусора)
                if (part.empty() || !std::all_of(part.begin(), part.end(), ::isdigit))
                {
                    valid = false;
                    break;
                }
                ip.push_back(std::stoi(part));
            }

            if (valid) ip_pool.push_back(ip);
        }

        // 1. Обратная лексикографическая сортировка
        std::sort(ip_pool.begin(), ip_pool.end(), [](const std::vector<int> &a, const std::vector<int> &b) {
            return a > b; 
        });

        // Весь список
        for (const auto &ip : ip_pool) print_ip(ip);

        // 2. Фильтрация: первый байт 1
        for (const auto &ip : ip_pool)
        {
            if (ip.at(0) == 1) print_ip(ip);
        }

        // 3. Фильтрация: 46.70.x.x (используем кортежи)
        for (const auto &ip : ip_pool)
        {
            if (std::tie(ip.at(0), ip.at(1)) == std::make_tuple(46, 70))
            {
                print_ip(ip);
            }
        }

        // 4. Фильтрация: любой байт 46 (используем decltype)
        using ip_item = decltype(ip_pool)::value_type;
        for (const ip_item &ip : ip_pool)
        {
            if (std::any_of(ip.begin(), ip.end(), [](int byte) { return byte == 46; }))
            {
                print_ip(ip);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}