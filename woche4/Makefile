
CC = avr-gcc
MCU = atmega328p
OBJS =  main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c \
		iescountdown.c check_lap.c linefollower.c
CFLAGS = -o tmpfile -mmcu=$(MCU) -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 \
		 -D LAPCOUNTER_MODE=1 -D LAPS=3 -Wall;

all: 

flash: compile
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex

compile:
	$(CC) $(OBJS) $(CFLAGS)
	avr-objcopy -O ihex tmpfile tmpfile.hex


documentation:

clean:






