cmake_minimum_required(VERSION 3.1)
project(Roboter C)

set(CMAKE_C_STANDARD 99)
set(MCU "atmega328p")
set(F_CPU "16000000")
set(CMAKE_SYSTEM_NAME Generic)

add_executable(Roboter "woche3/E3T4.c" "woche3/iesusart.h" "woche3/iesusart.c" "woche3/E3T2.c" woche4/iesmotors.c woche4/iesmotors.h woche4/skeleton_motors.c)
include_directories(include)
