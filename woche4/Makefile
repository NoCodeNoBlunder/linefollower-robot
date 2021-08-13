
MCU = atmega328p
CC = avr-gcc

flash: compile
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex

compile:
	avr-gcc main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c iescountdown.c \
	-o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=0 \
	-D LAPCOUNTER=1;
	avr-objcopy -O ihex tmpfile tmpfile.hex





