
FILE1=main.c
FILE2=states.c
FILE3=fsm.c

compile:
	avr-gcc main.c states.c fsm.c iesleds.c iesusart.c iesmotors.c iesadc.c util.c \
	-o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6;
	avr-objcopy -O ihex tmpfile tmpfile.hex

klara:
	avr-gcc $(FILE1) $(FILE2)

main.o:


states.o:


