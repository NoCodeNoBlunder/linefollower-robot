#include "iesmotors.h"
#include "typedefs.h"
#include "iesusart.h"
#include <stdio.h>
#include <util/delay.h>

// TODO macht kein sinn, da ich sowieso unterschiedliche Register nutze?!
typedef enum {
    IN1 = PD7,
    IN2 = PB0,
    IN3 = PB1,
    IN4 = PB3
} Engpolarity;

// Info is needed for PWM...?
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

// region POLARITY
void set_high(volatile char *reg, char pin) {
    reg[0] |= (1 << pin);
}

// TODO seperate into left and right side!
void set_low(volatile char *reg, char pin) {
    reg[0] &= ~(1 << pin);
}

void left_forward() {
    set_high(&PORTD, IN1);
    set_low(&PORTD, IN2);
}

void left_backward() {
    set_high(&PORTB, IN2);
    set_low(&PORTD, IN1);

}

void right_forward() {
    set_high(&PORTB, IN4);
    set_low(&PORTB, IN3);
}

void right_backward() {
    set_high(&PORTB, IN3);
    set_low(&PORTB, IN4);
}

void set_polarity_forward() {
    left_forward();
    right_forward();
}

void set_polarity_backward() {
    left_backward();
    right_backward();
}

void set_polarity_left_rot() {
    left_backward();
    right_forward();
}

void set_polarity_right_rot() {
    left_forward();
    right_backward();
}
// endregion

// region DUTY_CYCLE
void motors_Init() {
    // Delete everything on ports B and D
    DDRD = 0;
    DDRB = 0;

    // TODO alle pins müssen auf output gestellt sein.
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD = (1 << DD5) | (1 << DD6);

    // IN1
    DDRD |= (1 << DD7);

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB = (1 << DD0) | (1 << DD1) | (1 << DD3) | (1 << DD7);

    // Make PWM work on PD[5|6]
    setup_timer0();
}

// TODO Name change this?
void drive_straight(RoboterData *data) {
    // TODO Code wiederholung wie vermeide ich das?
    data ->left_eng_speed = ENG_MID;
    data ->right_eng_speed = ENG_MID;
    set_duty_cycle(LEFT_ENG, ENG_MID);
    set_duty_cycle(RIGHT_ENG, ENG_MID);
}

void turn_left(RoboterData *data) {
    data ->left_eng_speed = ENG_SLOW;
    data ->right_eng_speed = ENG_FAST;
    set_duty_cycle(LEFT_ENG, ENG_SLOW);
    set_duty_cycle(RIGHT_ENG, ENG_FAST);
}

void turn_right(RoboterData *data) {
    data ->left_eng_speed = ENG_FAST;
    data ->right_eng_speed = ENG_SLOW;
    set_duty_cycle(LEFT_ENG, ENG_FAST);
    set_duty_cycle(RIGHT_ENG, ENG_SLOW);
}

// TODO kann ich hieraus eine Couroutine schreiben die für mehrere Funktionen
// mit Pointern zur Funktion
// verwendbar ist?
void accelerate_straight(RoboterData *data, int to_value) {

    static int i, state = 0;
    switch (state) {
        case 0: goto M0;
        case 1: goto M1;
    }

    M0:
        // TODO warum brauche ich eine schneller accel rate?
    for (i = data -> right_eng_speed; i < to_value; i+=10) {
        state = 1;
        // TODO hier muss auch gemessen werden!
        set_duty_cycle(LEFT_ENG, i+1);
        set_duty_cycle(RIGHT_ENG, i+1);
        // _delay_ms(10);
        return;
        M1:;
    }

    USART_print("\nICH BIN FERTIG!\n");
}

void deaccelerate_straight(RoboterData *data, int to_value) {
    // Slowly decrease the duty
    for (unsigned char i = 255; i > to_value; i--) {
        set_duty_cycle(LEFT_ENG, i+1);
        set_duty_cycle(RIGHT_ENG, i+1);
//        _delay_ms(50);
    }
}

// TODO i dont think this is needed anymore!
void event_handler(RoboterData *data) {
    switch (data ->mode) {

        case IDLE:
            break;
        case FORWARD:
            break;
        case BACKWARD:
            break;
        case LEFT_TURN:
            break;
        case RIGHT_TURN:
            break;
        case LEFT_SPIN:
            break;
        case RIGHT_SPIN:
            break;
    }
}
// endregion










