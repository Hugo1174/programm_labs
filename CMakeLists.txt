cmake_minimum_required(VERSION 3.10)
project(ContainersLab)

# Указываем стандарт C++11 или выше для move-семантики
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исполняемый файл (замените main.cpp на имя вашего файла)
add_executable(containers_lab main.cpp)