cmake_minimum_required(VERSION 3.1)
project(Roboter C)

set(CMAKE_C_STANDARD 99)
set(MCU "atmega328p")
set(F_CPU "16000000")
set(CMAKE_SYSTEM_NAME Generic)

add_executable(Roboter "woche3/E3T4.c" "headers/iesusart.h" "woche3/iesusart.c" "woche3/E3T2.c" woche4/iesmotors.c headers/iesmotors.h woche4/skeleton_motors.c woche4/E4T3.c woche4/iesusart.c woche3/E3T3.c)
include_directories(include)
include_directories(headers)
