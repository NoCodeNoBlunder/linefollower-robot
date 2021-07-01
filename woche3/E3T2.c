#include "iesusart.h"
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HIGH "high"
#define LOW "low"
#define PB0_STR "PB0"
#define PB1_STR "PB1"
#define STR_BUF_SIZE 30

void init() {
    // Set Data Direction Register B0, B1  as input.
    DDRB &= ~((1<<DDB0) | (1<<DDB1));
    USART_init(UBRR_SETTING);
}


char get_is_bit_set(volatile char reg_offset, char pin_to_check) {
    return ((reg_offset & (1 << pin_to_check)) > 0);
}

void print_status_update(char* str_buf, char pinB0, char pinB1) {

    char *pinB0_value = pinB0 ? HIGH : LOW;
    char *pinB1_value = pinB1 ? HIGH : LOW;

    sprintf(str_buf, "%s is now set to: %s\n", PB0_STR, pinB0_value);
    USART_print(str_buf);
    sprintf(str_buf, "%s is now set to: %s\n\n", PB1_STR, pinB1_value);
    USART_print(str_buf);
}

int main(void) {

    init();

    char is_changed = 0;
    // Initialle Schalterstellung auslesen.
    char state_PB0 = get_is_bit_set(PINB, PB0);
    char state_PB1 = get_is_bit_set(PINB, PB1);
    char str_buf[STR_BUF_SIZE];
    print_status_update(str_buf ,state_PB0, state_PB1);

    while(1) {

        // Es soll nur was ausgegeben werden, wenn es eine Ãnderung gab.
        if(get_is_bit_set(PINB, PB0) != state_PB0) {
            state_PB0 ^= 1;
            is_changed = 1;
        }
        if(get_is_bit_set(PINB, PB1) != state_PB1){
            state_PB1 ^= 1;
            is_changed = 1;
        }

        if (is_changed) {
            print_status_update(str_buf, state_PB0, state_PB1);
            is_changed = 0;
        }
    }
    return 0;
}

