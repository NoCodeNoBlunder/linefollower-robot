

CC := avr-gcc
MCU := atmega328p
SRC_DIR := ./src/
BUILD_DIR := ./build/
SRC_FILES := $(wildcard $(SRC_DIR)*.c)

CFLAGS := -o $(BUILD_DIR)firmware -mmcu=$(MCU) -Os -Wall -D F_CPU=16E6 \
	-D CALIBRATION_MODE=1 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=0 -D LAPCOUNTER_MODE=0 -D LAPS=3;

all: compile hex flash

compile: $(SRC_FILES)
	$(CC) $(SRC_FILES) $(CFLAGS)

hex: compile
	avr-objcopy -O ihex $(BUILD_DIR)firmware $(BUILD_DIR)firmware.hex

flash: compile hex
	avrdude -c arduino -p $(MCU) -P /dev/ttyACM0 -b 115200 -U flash:w:$(BUILD_DIR)firmware.hex

init_doc:
	cd ./doc/; doxygen Doxyfile

update_doc:
	cd ./doc/; doxygen

open_doc:
	xdg-open ./doc/html/index.html &

clean:












