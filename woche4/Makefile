
MCU = atmega328p

flash: compile
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex

compile:
	avr-gcc main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c \
	-o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=0;
	avr-objcopy -O ihex tmpfile tmpfile.hex


main.o:
	avr-gcc main.c -o main.o -mmcu=atmega328p -Os -D F_CPU=16E6;

all: main.o states.o fsm.o iesleds.o iesusart.o iesmotors.o iesadc.o util.o

one:
	avr-gcc test.c -mmcu=atmega328p -Os -D F_CPU=16E6



