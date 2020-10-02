cmake_minimum_required(VERSION 3.15)
project(TP2 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

add_executable(TP2 main.c metodos.c metodos.h tests.c tests.h redirect.c redirect.h)