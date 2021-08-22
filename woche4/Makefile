

CC := avr-gcc
MCU := atmega328p
SRC_DIR := ./src/
SRC_FILES := $(wildcard $(SRC_DIR)*.c)


CFLAGS := -o ./build/tmpfile -mmcu=$(MCU) -Os -Wall -D F_CPU=16E6 \
	-D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 -D LAPCOUNTER_MODE=1 -D LAPS=3;


all: compile hex

compile: $(SRC_FILES)
	$(CC) $(SRC_FILES) $(CFLAGS)

hex:
	avr-objcopy -O ihex ./build/tmpfile ./build/tmpfile.hex

link:


flash:
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex










