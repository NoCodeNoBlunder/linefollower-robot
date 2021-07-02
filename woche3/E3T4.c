#include <avr/io.h>
#include <util/delay.h>
// #include <avr/iom328p.h>
#include "iesusart.h"

#define SHORT_WTIME 250
#define MID_WTIME 500
#define STR_BUF_SIZE 30
#define ASCII_OFFSET 48

typedef enum {
    NONE,
    LED1,
    LED2,
    LED3,
    ALL
} Diodes;

void toggle_volt(volatile char *group_offset, char element) {
    group_offset[0] ^= (1 << element);
}

void toggle_DI() {
    toggle_volt(&PORTB, PB2);
}

void send_flanks(int flank_count) {

    for (int i = 0; i < flank_count; i++) {
        toggle_volt(&PORTD, PD4);
        toggle_volt(&PORTD, PD4);
    }
}

void set_to_low(volatile char *group_offset, char element) {
    group_offset[0] &= ~(1 << element);
}

void reset() {
    set_to_low(&PORTB, PB2);
    send_flanks(3);
}

void fire_mode(Diodes diode) {

    reset();

    switch(diode) {
        case NONE:
            reset();
            break;
        case LED1:
            toggle_DI();
            send_flanks(1);
            break;
        case LED2:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case LED3:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(2);
            break;
        case ALL:
            toggle_DI();
            send_flanks(3);
            break;
        default:
            USART_print("There is no such mode!");
            break;
    }
}

void wait_ntimes(int n) {
    for(int i = 0; i < n; i++) {
        _delay_ms(SHORT_WTIME);
    }
}

int chr_to_int(char chr) {
    return (int)(chr) - ASCII_OFFSET;
}

void send_start_message() {
    USART_print("Type in word containing only integers:\n");
    USART_print("Each word consists of tupels (x,y).\n");
    USART_print("x := select Mode\n");
    USART_print("y := n (250ms) wait cycles\n");
    USART_print("0 : RESET   ||   1 : LED1\n");
    USART_print("2 : LED2     ||   3 : LED3\n");
    USART_print("4 : ALL\n");
}

void init() {
    USART_init(UBRR_SETTING);
    DDRB = (1 << DDB2);
    DDRD = (1 << DDD4);
}

// was wäre guter name dafuer?
void event_listener() {
    static char str_buf[STR_BUF_SIZE];
    char current = '\0';
    int len = 0;

    while(current != '\n') {
        current = USART_receiveByte();
        str_buf[len] = current;
        len++;
    }

    for (int j = 0; j < len-1; j += 2) {
        int mode = chr_to_int(str_buf[j]);
        fire_mode(mode);

        int wait_count = chr_to_int(str_buf[j+1]);
        wait_ntimes(wait_count);
        // reset();
    }

    // TODO Warum muss ich hier was transmitten???
    USART_transmitByte('\n');
}

int main(void) {

    init();
    send_start_message();

    while (1) {
        event_listener();
    }

    return 0;
}

