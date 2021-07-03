#include "iesmotors.h"
#include "typedefs.h"

// TODO macht kein sinn, da ich sowieso unterschiedliche Register nutze?!
typedef enum {
    IN1 = PD7,
    IN2 = PB0,
    IN3 = PB1,
    IN4 = PB3
} Engpolarity ;


/* sets up timer 0 (8 bit) */
void setup_timer0() {
    cli();                                  // disable interrupt globally // TODO why do interrupt need to be disabled?
    // Set Prescaler to 64
    TCCR0B = 0;
    TCCR0B |= (1 << CS00) | (1 << CS01);
    // Set waveform generation mode to Fast PWM, frequency = F_CPU / (PRESCALER * 2^8)
    TCCR0A = 0;
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    sei();                                  // enable interrupts globally
}

/* Sets duty-cycle at pin PD5 or PD6 (OC0A or OC0B)
 * to a value (0 - 255 = 0% - 100%).
 * Timer0 needs to be setup before usage.
 * The required pins also need to be set as output first. */
void set_duty_cycle(uint8_t pin, uint8_t value)
{
    // TODO Codewiederholunge?
    if (pin == PD6) {
        if (value == 0) {
            TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // normal port operation mode
            PORTD &= ~(1 << PD6);                       // PD6 LOW
        }
        else if (value == 255) {
            TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // normal port operation mode
            PORTD |= (1 << PD6);                        // PD6 HIGH
        }
        else {
            TCCR0A |= (1 << COM0A1);                    // Clear OC0A on Compare Match, set OC0A at
            TCCR0A &= ~(1 << COM0A0);                   // BOTTOM (non-inverting mode)
            OCR0A = value;
        }
    }
    if (pin == PD5) {
        if (value == 0) {
            TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);  // normal port operation mode
            PORTD &= ~(1 << PD5);                       // PD5 LOW
        }
        else if (value == 255) {
            TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);  // normal port operation mode
            PORTD |= (1 << PD5);                        // PD5 HIGH
        }
        else {
            TCCR0A |= (1 << COM0B1);                    // Clear OC0B on Compare Match, set OC0B at
            TCCR0A &= ~(1 << COM0B0);                   // BOTTOM (non-inverting mode)
            OCR0B = value;
        }
    }
}

void set_high(volatile char *reg, char pin) {
    reg[0] |= (1 << pin);
}

// TODO seperate into left and right side!
void set_low(volatile char *reg, char pin) {
    reg[0] &= ~(1 << pin);
}

void left_forward() {
    set_high(&PORTB, IN2);
    set_low(&PORTD, IN1);
}

void left_backward() {
    set_high(&PORTD, IN1);
    set_low(&PORTD, IN2);
}

void right_forward() {
    set_high(&PORTB, IN4);
    set_low(&PORTB, IN3);
}

void right_backward() {
    set_high(&PORTB, IN3);
    set_low(&PORTB, IN4);
}

void both_forward() {
    left_forward();
    right_forward();
}

void both_backward() {
    left_backward();
    right_backward();
}

void left_spin() {
    left_backward();
    right_forward();
}

void right_spin() {
    left_forward();
    right_backward();
}




