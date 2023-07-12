cmake_minimum_required(VERSION 3.25)
project(json C)

set(CMAKE_C_STANDARD 23)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

add_executable(json main.c json.h json.c list.h)
