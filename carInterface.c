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

void send_car_command(char *message, int *integer_data) {
    
    char returned[100] = {0}; // set to 0, otherwise picks up garbage
    char buff[100] = {0}; // set to 0, otherwise picks up garbage
    char *data = "";
    int data_count;
    char *garbage_characters;
    
    while (strlen(returned) <= 1){
        write_uart(message);
        read_uart(returned, 100);
    }
    write_uart(" Received 1 "); // debug
     
    while (strchr(buff, '>') == NULL) {
        read_uart(buff, 100);
    }
    write_uart(" Received 2 "); // debug
    
    data = strtok(returned, " ");
    
    data_count = 0;
    while (data != NULL) {
        data_count++;
        // do nothing with the first two arguments of the returned data
        if (data_count <= 2) {
            data = strtok(NULL, " ");
            continue;
        }

        integer_data[data_count - 3] = strtol(data, &garbage_characters, 16);
        data = strtok(NULL, " ");
    }
      
    return;
}
    