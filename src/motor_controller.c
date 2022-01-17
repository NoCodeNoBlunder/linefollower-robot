/**
 * @file
 * @brief Controlls everything regarding the motors of the robot
 * @date 2021-06-08
 */

#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../inc/main.h"
#include "../inc/motor_controller.h"
#include "../inc/fsm.h"
#include "../inc/usart.h"
#include "../inc/util.h"

/**
 * @enum
 * @brief
 * Pin constants for to controll the engines polarity
 */
enum {
    IN1 = PD7,
    IN2 = PB0,
    IN3 = PB1,
    IN4 = PB3
};

/**
 * @enum
 * @breif
 * Constants for the engine duty cycles to be set according to drive mode.
 */
enum {
    ENG_STILL = 0,
    ENG_FORWARD = 120,
    ENG_SOFT_SLOW = 60,
    ENG_SOFT_FAST = 220,
    ENG_HARD_BACK = 155,
    ENG_HARD_FORWARD = 255,
    ENG_MAX = 255,
};

/**
 * @enum
 * @brief
 * Pin constants that are responsible for the left and right engine
 */
enum {
    LEFT_ENG = PD5,
    RIGHT_ENG = PD6
};

// Info is needed for PWM...?
/* sets up timer 0 (8 bit) */
void Timer0_init() {
    cli();                                  // disable interrupt globally
    // Set Prescaler to 64
    TCCR0B = 0;
    TCCR0B |= (1 << CS00) | (1 << CS01);
    // Set waveform generation mode to Fast PWM, frequency = F_CPU / (PRESCALER * 2^8)
    TCCR0A = 0;
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    sei();                                  // enable interrupts globally
}

/** Sets duty-cycle at pin PD5 or PD6 (OC0A or OC0B)
 * to a value (0 - 255 = 0% - 100%).
 * Timer0 needs to be setup before usage.
 * The required pins also need to be set as output first.
 */
void set_duty_cycle(uint8_t pin, uint8_t value)
{
    if (pin == RIGHT_ENG) {
        if (value == ENG_STILL) {
            TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // normal port operation mode
            PORTD &= ~(1 << RIGHT_ENG);                 // PD6 LOW
        }
        else if (value == ENG_MAX) {
            TCCR0A &= ~(1 << COM0A1) & ~(1 << COM0A0);  // normal port operation mode
            PORTD |= (1 << RIGHT_ENG);                        // PD6 HIGH
        }
        else {
            TCCR0A |= (1 << COM0A1);                    // Clear OC0A on Compare Match, set OC0A at
            TCCR0A &= ~(1 << COM0A0);                   // BOTTOM (non-inverting mode)
            OCR0A = value;
        }
    }
    if (pin == LEFT_ENG) {
        if (value == ENG_STILL) {
            TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);  // normal port operation mode
            PORTD &= ~(1 << LEFT_ENG);                       // PD5 LOW
        }
        else if (value == ENG_MAX) {
            TCCR0A &= ~(1 << COM0B1) & ~(1 << COM0B0);  // normal port operation mode
            PORTD |= (1 << LEFT_ENG);                   // PD5 HIGH
        }
        else {
            TCCR0A |= (1 << COM0B1);                    // Clear OC0B on Compare Match, set OC0B at
            TCCR0A &= ~(1 << COM0B0);                   // BOTTOM (non-inverting mode)
            OCR0B = value;
        }
    }
}

void left_forward() {
    set_pin_high(&PORTD, IN1);
    set_pin_low(&PORTB, IN2);
}

void left_backward() {
    set_pin_high(&PORTB, IN2);
    set_pin_low(&PORTD, IN1);
}

void right_forward() {
    set_pin_high(&PORTB, IN4);
    set_pin_low(&PORTB, IN3);
}

void right_backward() {
    set_pin_high(&PORTB, IN3);
    set_pin_low(&PORTB, IN4);
}

void left_off() {
    set_pin_low(&PORTD, IN1);
    set_pin_low(&PORTB, IN2);
}

void right_off() {
    set_pin_low(&PORTB, IN4);
    set_pin_low(&PORTB, IN3);
}

/**
 * Sets the polarity of the engines, determining in which direction one side is spinning.
 * @param dir to select the polarity for the desired driving behaviour.
 */
void set_polarity(State dir) {
    switch (dir) {
        case FORWARD:
            left_forward();
            right_forward();
            break;
        case LEFT_HARD:
            left_backward();
            right_forward();
            break;
        case RIGHT_HARD:
            left_forward();
            right_backward();
            break;
        case LEFT_SOFT:
            left_forward();
            right_forward();
            break;
        case RIGHT_SOFT:
            left_forward();
            right_forward();
            break;
        case CHECK_LAP:
            left_forward();
            right_forward();
            break;
        case GOAL_REACHED:
            left_forward();
            right_forward();
            break;
        case STILL:
            left_off();
            right_off();
            break;
        case EXIT:
            left_off();
            right_off();
            break;
        default:
            break;
    }
}

void motors_Init() {
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD |= (1 << DD5) | (1 << DD6);

    // IN1
    DDRD |= (1 << DD7);

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB |= (1 << DD0) | (1 << DD1) | (1 << DD3) | (1 << DD7);

    // Make PWM work on PD[5|6]
    Timer0_init();
}

void set_direction(RoboterData *data, State state) {

    switch (state) {
        case FORWARD:
            data->left_eng_speed = ENG_FORWARD;
            data->right_eng_speed = ENG_FORWARD;
            break;
        case LEFT_SOFT:
            data->left_eng_speed = ENG_SOFT_SLOW;
            data->right_eng_speed = ENG_SOFT_FAST;
            break;
        case RIGHT_SOFT:
            data->left_eng_speed = ENG_SOFT_FAST;
            data->right_eng_speed = ENG_SOFT_SLOW;
            break;
        case LEFT_HARD:
            data->left_eng_speed = ENG_HARD_BACK;
            data->right_eng_speed = ENG_HARD_FORWARD;
            break;
        case RIGHT_HARD:
            data->left_eng_speed = ENG_HARD_FORWARD;
            data->right_eng_speed = ENG_HARD_BACK;
            break;
        case CHECK_LAP:
            data->left_eng_speed = ENG_SOFT_SLOW;
            data->right_eng_speed = ENG_SOFT_SLOW;
            break;
        case STILL:
            data->left_eng_speed = ENG_STILL;
            data->right_eng_speed = ENG_STILL;
            break;
        default:
            break;
    }

    set_polarity(state);
    set_duty_cycle(LEFT_ENG, data->left_eng_speed);
    set_duty_cycle(RIGHT_ENG, data->right_eng_speed);
}










