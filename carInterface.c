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
#include "ctype.h"

#include "carInterface.h"
#include "main.h"

#endif

#define MAX_BYTES 16

void UARTSetup(void) {
    char message[100];
    
    RPB3Rbits.RPB3R = 0b0001; // pin B3 set to TX
    U1RXRbits.U1RXR = 0b0100; // pin B2 set to RX
    U1MODEbits.BRGH = 0;
    U1BRG = ((30000000 / 9600) / 8) - 1; // baud rate
    U1MODEbits.PDSEL = 0b00; // parity bits (0 = 8 bit, no parity)
    U1MODEbits.STSEL = 0; // stop bits (0 = 1 stop bit)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
    
    // initialize the car connection
    sprintf(message, "atz\r");
    write_uart(message);
    wait_for_obd();
    
    // discover obd2 protocol
    sprintf(message, "atsp0\r");
    write_uart(message);  
    wait_for_obd();
    
    // pull data once, first time requires searching
    sprintf(message, "010C\r"); // request RPM
    write_uart(message);
    wait_for_obd();
}

void write_uart(char * string) {
    while (*string != '\0') {
            while (U1STAbits.UTXBF);
            U1TXREG = *string;
            ++string;
        }
}

void read_uart(char *response){
    
  int index = 0;
  int complete = 0;
  char read_char;
  
  while (!complete) {
    if (U1STAbits.URXDA) {
      read_char = U1RXREG;

      if (read_char == '\r') {
          response[index] = '\0';
          complete = 1;
      }
      else {
          response[index++] = read_char;
      }
    }
  }
}

void parse_car_data(char *response, int *integer_data) {
      
    int num_bytes = 0;
    int valid_hex = 1;

    char *token = strtok(response, " ");  // tokenize the string by spaces

    while (token != NULL && num_bytes < MAX_BYTES) {
        
        // check if all characters in token are hex numbers
        valid_hex = 1;
        for (int i = 0; i < strlen(token); i++) {
            if (!isxdigit(token[i])) {
                valid_hex = 0;
                break;
            }
        }

        if (valid_hex) {
            // convert token from hex string to integer
            integer_data[num_bytes] = strtol(token, NULL, 16);
            num_bytes++;
        }

        token = strtok(NULL, " ");  // Get the next token
    }
    
    return;
}

void wait_for_obd(void) {
    char ret_char = '0'; // placeholder for first read
    int timeout_time = 25000000; // about 10s
    int timeout_counter = timeout_time;
    
    while (ret_char != '>') {
        if (U1STAbits.URXDA) {
            timeout_counter = timeout_time;
            ret_char = U1RXREG;
        }
        else {
            timeout_counter--;
            if (timeout_counter <= 0) {
                __pic32_software_reset();
            }
        }
    }
}

void retrieve_data(char *message, int *integer_data) {
    char response[100];
    
    write_uart(message);

    read_uart(response); // collects the echo
    read_uart(response); // collects the data

    wait_for_obd(); // wait for obd to be ready, make sure nothing went wrong
    
    parse_car_data(response, integer_data);
    
    return;
}
    