cmake_minimum_required(VERSION 3.1)
project(Roboter C)

set(CMAKE_C_STANDARD 99)
set(MCU "atmega328p")
set(F_CPU "16000000")
set(CMAKE_SYSTEM_NAME Generic)

add_executable(Roboter "inc/adc.h" "inc/check_lap_state.h" "inc/countdown_states.h"  "inc/fsm.h" "inc/led_controller.h" "inc/linefollower.h" "inc/main.h" "inc/motor_controller.h" "inc/states.h" "inc/usart.h" "inc/util.h" "src/adc.c" "src/check_lap_state.c" "src/countdown_states.c" "src/fsm.c" "src/led_controller.c" "src/linefollower.c" "src/main.c" "src/motor_controller.c" "src/states.c" "src/usart.c" "src/util.c")
include_directories(inc)
include_directories(src)
