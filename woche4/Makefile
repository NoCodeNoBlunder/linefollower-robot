

CC := avr-gcc
MCU := atmega328p
SRC_DIR := ./src/
# SRC_FILES = $(SRC_DIR)main.c $(SRC_DIR)states.c $(SRC_DIR)fsm.c $(SRC_DIR)iesleds.c $(SRC_DIR)iesusart.c $(SRC_DIR)iesmotors.c $(SRC_DIR)iesadc.c $(SRC_DIR)util.c \
       		$(SRC_DIR)iescountdown.c $(SRC_DIR)check_lap.c $(SRC_DIR)linefollower.c
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)


CFLAGS := -o ./build/tmpfile -mmcu=$(MCU) -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 \
		 -D LAPCOUNTER_MODE=1 -D LAPS=3 -Wall;

all: compile hex

compile: $(SRC_FILES)
	$(CC) $(SRC_FILES) $(CFLAGS)

hex:
	avr-objcopy -O ihex ./build/tmpfile ./build/tmpfile.hex

link:


flash:
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex










