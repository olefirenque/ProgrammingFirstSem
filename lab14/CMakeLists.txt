cmake_minimum_required(VERSION 3.17)
project(gameOfLife C)

set(CMAKE_C_STANDARD 11)

add_executable(gameOfLife main.c bmp.c bmp.h)