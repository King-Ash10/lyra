cmake_minimum_required(VERSION 3.20)

project(lyra)

set(CMAKE_CXX_STANDARD 23)

find_package(ftxui REQUIRED)
find_package(fmt REQUIRED)

add_executable(lyra
    src/main.cpp
)

target_link_libraries(lyra PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component

    fmt::fmt
)
