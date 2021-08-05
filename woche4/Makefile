
FILE1=main.c
FILE2=states.c
FILE3=fsm.c

flash: compile
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex

compile:
	avr-gcc main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c \
	-o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6;
	avr-objcopy -O ihex tmpfile tmpfile.hex


main.o:
	avr-gcc main.c -o main.o -mmcu=atmega328p -Os -D F_CPU=16E6;

all: main.o states.o fsm.o iesleds.o iesusart.o iesmotors.o iesadc.o util.o

