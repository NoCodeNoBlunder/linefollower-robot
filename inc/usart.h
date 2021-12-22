/**
 * @file
 * @brief Basic serial communication via USART for ATMEGA328
 * @version 0.1
 * @date 2021-06-08
 */

#ifndef IESUSART_h
#define IESUSART_h

#include "main.h"

/**
 * @def
 * determines how many symbols are send per second
 */
#define BAUD 9600

/**
 * @def determines what to write into the UBRR register
 */
#define UBRR_SETTING F_CPU/16.0/BAUD-1

/**
 * @brief
 * Reads a single byte out of the USART receive buffer.
 * A good way to use this would be calling it from an receive interrupt serice routine
 * @return received byte
 */
unsigned char USART_receiveByte(void);

/**
 * @brief Writes a single byte to the USART transmit buffer
 * @param data Byte that shall be transmitted
 */
void USART_transmitByte(unsigned char data);

/**
 * @brief Transmittes a string (char by char) until '\0’ is reached
 * @param c pointer to the string to transmit
 */
void USART_print(const char *c);

/**
 * @brief Sets up the USART port (The USART baudrate register)
 * @param ubrr Content to write into the UBRR register
 */
void USART_Init(unsigned long ubrr);

/**
 * @brief Transmits data stored @date via Serial Port if DEBUG_MODE is enabled.
 * This is used for debugging purposes.
 * @param fsm passes the current state stored in the fsm instance.
 * @param data passes sensor values and engines speed values to transmit.
 * @details if DEBUG_MODE is enabled it severly decreases the roboters performance
 * as transmitting via Serial Port is very expensive.
 */
void transmit_debug_msg(FSM *fsm, RoboterData *data);
#endif
