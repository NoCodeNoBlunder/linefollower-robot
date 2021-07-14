

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "iesleds.h"
#include "iesusart.h"
#include "util.h"

#define SHORT_WTIME 250
#define MID_WTIME 500
#define ASCII_OFFSET 48
#define SR_LED_COUNT 3
void toggle_DI() {
    toggle_pin(&PORTB, PB2);
}

void send_flanks(char flank_count) {
    for (char i = 0; i < flank_count; i++) {
        toggle_pin(&PORTD, PD4);
        toggle_pin(&PORTD, PD4);
    }
}

void reset() {
    set_pin_low(&PORTB, PB2);
    send_flanks(SR_LED_COUNT);
}

void light_led(Sensor diode) {

    // TODO muss ich immer resetten?
    reset();

    switch (diode) {
        case LEFT_LF:
            toggle_DI();
            send_flanks(1);
            break;
        case MID_LF:
            toggle_DI();
            send_flanks(1);
            toggle_DI();
            send_flanks(1);
            break;
        case RIGHT_LF:
            light_led(MID_LF);
            send_flanks(1);
            break;
        case LEFT_AND_RIGHT:
            light_led(MID_LF);
            toggle_DI();
            send_flanks(1);
            break;
        case NONE:
            break;
    }

//    switch(diode) {
//        case NONE:
//            reset();
//            break;
//        case LED_LEFT_LF:
//            toggle_DI();
//            send_flanks(1);
//            break;
//        case LED_RIGHT_LF:
//            toggle_DI();
//            send_flanks(1);
//            toggle_DI();
//            send_flanks(1);
//            break;
//        case LED_MID_LF:
//            toggle_DI();
//            send_flanks(1);
//            toggle_DI();
//            send_flanks(2);
//            break;
//        case ALL:
//            toggle_DI();
//            send_flanks(3);
//            break;
//        default:
//            USART_print("There is no such mode!");
//            break;
//    }
}

void leds_Init() {
    DDRB = (1 << DDB2);
    DDRD = (1 << DDD4);
}

//void wait_ntimes(int n) {
//    for(int i = 0; i < n; i++) {
//        _delay_ms(SHORT_WTIME);
//    }
//}

//int chr_to_int(char chr) {
//    return (int)(chr) - ASCII_OFFSET;
//}
//
//// was wäre guter name dafuer?
//void event_listener() {
//    static char str_buf[STR_BUF_SIZE];
//    char current = '\0';
//    int len = 0;
//
//    while(current != '\n') {
//        current = USART_receiveByte();
//        str_buf[len] = current;
//        len++;
//    }
//
//    for (int j = 0; j < len-1; j += 2) {
//        int mode = chr_to_int(str_buf[j]);
//        light_led(mode);
//
//        int wait_count = chr_to_int(str_buf[j+1]);
//        wait_ntimes(wait_count);
//        // reset();
//    }
//
//    // TODO Warum muss ich hier was transmitten???
//    USART_transmitByte('\n');
//}

//int main(void) {
//
//    send_start_message();
//
//    while (1) {
//        event_listener();
//    }
//
//    return 0;
//}

