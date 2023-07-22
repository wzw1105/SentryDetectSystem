cmake_minimum_required(VERSION 3.5)

project(qtsystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(qtsystem mainwindow.cpp)

install(TARGETS qtsystem
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
