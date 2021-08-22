

CC := avr-gcc
MCU := atmega328p
SRC_DIR := ./src/
BUILD_DIR := ./build/
SRC_FILES := $(wildcard $(SRC_DIR)*.c)

CFLAGS := -o $(BUILD_DIR)firmware -mmcu=$(MCU) -Os -Wall -D F_CPU=16E6 \
	-D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 -D LAPCOUNTER_MODE=1 -D LAPS=3;


all: compile hex flash

compile: $(SRC_FILES)
	$(CC) $(SRC_FILES) $(CFLAGS)

hex:
	avr-objcopy -O ihex $(BUILD_DIR)firmware $(BUILD_DIR)firmware.hex

link:


flash:
	avrdude -c arduino -p $(MCU) -P /dev/ttyACM0 -b 115200 -U flash:w:$(BUILD_DIR)firmware.hex

documentation:
	cd ./doc/; doxygen

open_doc:
	xdg-open ./doc/html/index.html &

clean:












