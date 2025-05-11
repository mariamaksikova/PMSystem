cmake_minimum_required(VERSION 3.10)
project(HotelSystem)

set(CMAKE_CXX_STANDARD 17)

# Указываем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/Crow/include
    ${CMAKE_SOURCE_DIR}/src
)

# Добавляем исходные файлы
add_executable(pms_system
    ${CMAKE_SOURCE_DIR}/main.cpp
)

# Подключаем зависимости
find_package(Threads REQUIRED)
target_link_libraries(pms_system PRIVATE Threads::Threads)