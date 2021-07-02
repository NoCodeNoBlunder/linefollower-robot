cmake_minimum_required(VERSION 3.1)
project(Roboter C)

set(CMAKE_C_STANDARD 99)

add_executable(Roboter "woche3/E3T4.c" "woche3/iesusart.h" "woche3/iesusart.c" "woche3/E3T2.c")
include_directories(include)
