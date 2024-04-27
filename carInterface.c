/* 
 * File:   rtcc.c
 * Author: wapta
 *
 * Created on February 10, 2024, 1:52 PM
 */

#ifndef _carInterface_C_
#define _carInterface_C_

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include "p32xxxx.h"
#include <sys/attribs.h>
#include "string.h"

#include "carInterface.h"
#include "main.h"

#endif

void UARTSetup(void) {
    RPB3Rbits.RPB3R = 0b0001; // pin B3 set to TX
    U1RXRbits.U1RXR = 0b0100; // pin B2 set to RX
    U1MODEbits.BRGH = 0;
    U1BRG = ((30000000 / 9600) / 8) - 1; // baud rate
    U1MODEbits.PDSEL = 0b00; // parity bits (0 = 8 bit, no parity)
    U1MODEbits.STSEL = 0; // stop bits (0 = 1 stop bit)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}

void write_uart(char * string) {
    while (*string != '\0') {
            while (U1STAbits.UTXBF) {
                ;
            }
            U1TXREG = *string;
            ++string;
        }
}

void read_uart(char * message, int maxLength) {
    char data = 0;
    int complete = 0, num_bytes = 0;
    
    while (!complete) {
        if (U1STAbits.URXDA) {
            data = U1RXREG;
            if ((data == '\n') || (data == '\r')) {
                complete = 1;
            }
            else {
                message[num_bytes] = data;
                ++num_bytes;
                if (num_bytes >= maxLength) {
                    num_bytes = 0;
                }
            }
        }
    }
    message[num_bytes] = '\0';
}

void initialize_car_connection(void) {
    
    int i = 0;
    int j = 0;
    char message[100];
    char returned[100];
    
    sprintf(message, "atz");
    while (strstr(returned, "ELM327 v1.4b") == NULL){
        write_uart(message);
        delay(1000);
        read_uart(returned, 100);
    }
    
    while (strchr(returned, '>') == NULL) {
        read_uart(returned, 100);
    }
    
    sprintf(message, "atsp0");
    while (strstr(returned, "OK") == NULL){
        write_uart(message);
        delay(1000);
        read_uart(returned, 100);
    }
    
    while (strchr(returned, '>') == NULL) {
        read_uart(returned, 100);
    }
    
    return;

}
    