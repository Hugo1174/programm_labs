cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 1.0.${PATCH_VERSION})

# Установка patch версии
project(helloworld VERSION ${PROJECT_VERSION})

# Поиск Boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

# Создание целей
add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)

set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# путь к исполняемым файлам
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# Подключение библиотеки к исполняемым файлам
target_link_libraries(helloworld_cli PRIVATE
    helloworld
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld
)

# флаги для проверок
if (MSVC)
    target_compile_options(helloworld_cli PRIVATE /W4)
    target_compile_options(helloworld PRIVATE /W4)
    target_compile_options(test_version PRIVATE /W4)
else ()
    target_compile_options(helloworld_cli PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(helloworld PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Правила для установки собранной программы 
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

# --- НАСТРОЙКИ CPACK ---

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "sfd1@tpu.ru")

# Логика выбора генератора в зависимости от ОС
if(APPLE)
    # На macOS создаем ZIP-архив (наиболее универсально для CLI)
    # Можно заменить на "DragNDrop" для создания .dmg
    set(CPACK_GENERATOR "ZIP")
else()
    # На Linux оставляем DEB
    set(CPACK_GENERATOR "DEB")
endif()

include(CPack)

# Включение тестирования
enable_testing()
add_test(test_version test_version)