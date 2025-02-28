cmake_minimum_required(VERSION 3.16)
project(untitled C)

add_executable(untitled main.c)

target_link_libraries(untitled PRIVATE gmp)

