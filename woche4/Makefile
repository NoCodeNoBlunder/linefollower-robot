
MCU = atmega328p
CC = avr-gcc
OBJS =  main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c \
		iescountdown.c check_lap.c linefollower.c
CFLAGS = -o tmpfile -mmcu=$(MCU) -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=0 \
		 -D LAPCOUNTER_MODE=0 -D LAPS=3 -Wall;

flash: compile
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex

compile:
	$(CC) $(OBJS) $(CFLAGS)
	avr-objcopy -O ihex tmpfile tmpfile.hex

clean:





