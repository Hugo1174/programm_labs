cmake_minimum_required(VERSION 3.10) # Убрали устаревание

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION}) # Исправлена опечатка
project(physics VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CPACK_PROJECT_NAME physics)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Для корректной работы на macOS и Linux добавляем window и system
find_package(SFML 2.5 COMPONENTS graphics window system)

if (SFML_FOUND)
    message(STATUS "The SFML installed in the system will be used")
else ()
    message(STATUS "Get SFML from the source code")
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()

if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

add_executable(physics
        main.cpp World.cpp Ball.cpp Physics.cpp Color.cpp Painter.cpp Dust.cpp
        black_box/Application.cpp black_box/View.cpp black_box/PainterImpl.cpp)

target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

# Настройки CPack
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-3-dev libstdc++6-dev")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "example@example.com")

include(CPack)

enable_testing()