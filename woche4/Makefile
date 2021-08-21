
CC := avr-gcc
MCU := atmega328p
SRC := ./src
OBJ := ./obj
SRCS := $(wildcard $(SRC)/*.c) 					   # finds all .c files in the directory
OBJS := $(patsubst $(SRC)%.c, $(SRC)%.o, $(SRCS))  # change list of source files into list of object files replacing suffix .c with .o

CFLAGS := -o tmpfile -mmcu=$(MCU) -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 \
		 -D LAPCOUNTER_MODE=1 -D LAPS=3 -Wall;

all: compile hex flash

compile: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS)

hex:
	avr-objcopy -O ihex tmpfile tmpfile.hex

link:


flash:
	avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex






