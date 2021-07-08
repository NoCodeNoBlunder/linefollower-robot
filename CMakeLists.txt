cmake_minimum_required(VERSION 3.1)
project(Roboter C)

set(CMAKE_C_STANDARD 99)
set(MCU "atmega328p")
set(F_CPU "16000000")
set(CMAKE_SYSTEM_NAME Generic)

add_executable(Roboter "woche3/E3T4.c" "woche4/iesusart.h" "woche3/iesusart.c" "woche3/E3T2.c" woche4/iesmotors.c woche4/iesmotors.h woche4/main.c woche4/iesusart.c woche3/E3T3.c woche4/iesadc.c woche4/iesadc.h woche3/StructInfo.c woche4/typedefs.h woche4/fsm.c woche4/fsm.h woche4/compile.sh)
include_directories(include)
include_directories(headers)
