#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../lib/stdio.h"
#include "iesusart.h"
#define ECHO PB4
#define TRIGGER PB5

unsigned int cnt = 0;
unsigned int echo_start = 0;
unsigned int echo_end = 0;
unsigned int echo_duration = 0;

ISR (PCINT0_vect) {
    if (PINB & (1 << ECHO)) {
        // JUST RISEN
        echo_start = cnt;
    } else {
        // JUST FALLEN
        if (cnt < echo_start) {
            // Es gab einen Überlauf
            // Need to do something pretty here.

        }
        echo_duration = cnt - echo_start; // eigentlich Anzahl der overflows.
    }
}

ISR (TIMER2_COMPA_vect) {
    cnt+=1;
}

void setupTimer2() {
    cli();
    TCCR2B = (1 << CS00);  // Prescaler: 1
    TIMSK2 |= (1 << OCIE2A);
    TCCR2A = (1 << WGM01);
    TCNT2 = 0;
    OCR2A = 255;
    sei();
}

void setupPCINTPB4() {
    cli();                   // disable interrupts globally
    PCICR |= (1 << PCIE0);   // Gruppe von pins ist als Pin change enabled.
    PCMSK0 |= (1 << PCINT4); // Pin change interrupt enablen.
    sei();                   // enable interrupts globally
}

int main(void) {
    DDRB |= (1 << TRIGGER); // TRIGGER Pins als Ausgang
    DDRB &= ~(1 << ECHO);   // ECHO Pin als Eingang

    USART_Init(UBRR_SETTING);

    setupTimer2();

    setupPCINTPB4();

    while(1) {
        PORTB &= ~(1 << TRIGGER); // hierppofabian wird kein Trigger ausgesendet.
        _delay_us(10);
        PORTB |= (1 << TRIGGER);
        _delay_us(10);
        PCMSK0 |= (1 << PCINT4); // Wieso wird hier der Interrupt eingeschaltet??
        cnt = 0;
        PCMSK0 &= ~(1 << PCINT4); // maske geloescht damit kein weiter interrupt gestartet wird.
        PORTB &= ~(1 << TRIGGER); // erst nach 250 us wird eine Echo ausgesant.
        _delay_us(2);         //
        PCMSK0 |= (1 << PCINT4); // interrupt wird wieder einstellt. Trigger
        _delay_ms(100);      //

        char strbuff[17];

        USART_print("ECHO DURATION: ");
        sprintf(strbuff, "%u", echo_duration);
        USART_print(strbuff);

        float ticks_per_of = 16E6 / 256.0;    // Frequentz von Overflow in Hz
        float of_per_sec = 1 / ticks_per_of; // Dauer von einem overflow.
        float m_per_sec = 333.0;             // Geschwindigkeit vom Schall.
        float distance = echo_duration * of_per_sec * m_per_sec / 2.0 * 100;
        int idis = (int) distance;

        USART_print("     OBSTACLE DISTANCE: ");
        sprintf(strbuff, "%u", idis);
        USART_print(strbuff);
        USART_print("\n");
    }
    return 0;
}