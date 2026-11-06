cmake_minimum_required(VERSION 3.15)
project(CoursovayaRabota LANGUAGES C CXX)     # C нужен для sqlite3.c

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SQLite как отдельная библиотека
add_library(sqlite3 third_party/sqlite3/sqlite3.c)
target_include_directories(sqlite3 PUBLIC third_party/sqlite3)

# Все .cpp из src/ (CONFIGURE_DEPENDS: новые файлы подхватываются сами)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(CoursovayaRabota ${SOURCES})
target_include_directories(CoursovayaRabota PRIVATE src)
target_link_libraries(CoursovayaRabota PRIVATE sqlite3)

# Исходники в UTF-8, чтобы русский текст в консоли не превращался в кракозябры
if(MSVC)
    target_compile_options(CoursovayaRabota PRIVATE /utf-8)
endif()
