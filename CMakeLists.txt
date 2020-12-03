cmake_minimum_required(VERSION 3.17)
project(zzz)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(Sdsl REQUIRED STATIC)

add_executable(main main.cpp)

target_link_libraries(main Sdsl)
