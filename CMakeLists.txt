cmake_minimum_required(VERSION 3.13)

project(ParallAlgo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parall_algo src/main.cpp)

find_package(benchmark REQUIRED)

target_link_libraries(parall_algo PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(parall_algo PUBLIC include)