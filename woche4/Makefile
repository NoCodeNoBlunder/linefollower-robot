
CC := avr-gcc
MCU := atmega328p
SRC := ./src
OBJ := ./obj
 # SRCS := $(wildcard $(SRC)/*.c) 					   # finds all .c files in the directory
OBJS := $(patsubst $(SRC)%.c, $(SRC)%.o, $(SRCS))  # change list of source files into list of object files replacing suffix .c with .o

SRCS =  ./src/main.c ./src/states.c ./src/fsm.c ./src/iesleds.c ./src/iesusart.c ./src/iesmotors.c ./src/iesadc.c ./src/util.c \
		./src/iescountdown.c ./src/check_lap.c ./src/linefollower.c

CFLAGS := -o tmpfile -mmcu=$(MCU) -Os -D F_CPU=16E6 -D DEBUG_MODE=0 -D COUNTDOWN_MODE=1 \
		 -D LAPCOUNTER_MODE=1 -D LAPS=3 -Wall;


all: compile link $(TARGET).hex flash

$(TARGET).hex: $(TARGET)
	avr-objcopy -O ihex $< $@

link: $(OBJS)
	$(CC) -Wall -mmcu=$(MCU) $^ -o $(TARGET)

compile: $(SRCS)
	$(CC) $(CFLAGS)





