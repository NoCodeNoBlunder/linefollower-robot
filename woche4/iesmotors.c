/**
 * @file iesmotors.c
 * @brief Controlls everything regarding the motors of the robot
 * @version 0.1
 * @date 2021-06-08
 */

#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"
#include "iesmotors.h"
#include "fsm.h"
#include "iesusart.h"
#include "util.h"

enum {
    IN1 = PD7,
    IN2 = PB0,
    IN3 = PB1,
    IN4 = PB3
};

enum {
    ENG_STILL = 0,
    ENG_SLOW = 140,
    ENG_MID = 150, // wenn er zu schnell faehrt geht er zu spaet in hard turn
    ENG_FAST = 180,
    ENG_MAX = 255,
};

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
            right_forward();
            left_forward();
            break;
        case EXIT:
            left_forward();
            right_forward();
            break;
        default:
            break;
    }
}

void motors_Init() {
    // TODO alle pins müssen auf output gestellt sein.
    // Set PD5 and PD6 as output (EN[A|B]!)
    DDRD |= (1 << DD5) | (1 << DD6);

    // IN1
    DDRD |= (1 << DD7);

    // Set PB0, PB1, and PB3 as output (IN[2|3|4])
    DDRB |= (1 << DD0) | (1 << DD1) | (1 << DD3) | (1 << DD7);

    // Make PWM work on PD[5|6]
    Timer0_init();
}

// TODO maybe there should be an enum setting for the motors and not a shared one??
/**
 * Function to set engines polarity and duty_cyles to control the Roboter drive direction.
 */
void set_direction(RoboterData *data, State state) {
	
	// USART_print("\nset_direction was called\n");
    switch (state) {
        case LEFT_HARD:
            data->left_eng_speed = ENG_SLOW;
            data->right_eng_speed = ENG_FAST;
            break;
        case RIGHT_HARD:
            data->left_eng_speed = ENG_FAST;
            data->right_eng_speed = ENG_SLOW;
            break;
        case FORWARD:
            data->left_eng_speed = ENG_MID;
            data->right_eng_speed = ENG_MID;
            break;
        case LEFT_SOFT:
			data->left_eng_speed = ENG_SLOW; // Alternative
			data->right_eng_speed = ENG_MID;
			break;
        case RIGHT_SOFT:
			data->left_eng_speed = ENG_MID;
			data->right_eng_speed = ENG_SLOW;
			break;
        case CHECK_LAP:
            data->left_eng_speed = ENG_SLOW;
            data->right_eng_speed = ENG_SLOW;
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










