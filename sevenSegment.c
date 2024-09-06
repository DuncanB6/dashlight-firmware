/* 
 * File:   sevenSegment.c
 * Author: wapta
 *
 * Created on February 10, 2024, 1:32 PM
 */
#ifndef _sevenSegment_C_
#define _sevenSegment_C_

#include "xc.h"
#include "p32xxxx.h"
#include <stdio.h>
#include <sys/attribs.h>
#include <stdlib.h>

#include "main.h"
#include "sevenSegment.h"
#include "init_timers.h"

#endif

// For 7 seg interrupt
uint8_t digs[4]; // Digits to be written to 7 seg
int digit_index = 0; // Index for 7 seg digit, cycles through digits

uint8_t convert_to_bin(char dig, int colon_on) {
    /*
    * Writes a single digit to the 7 segment by shifting it into registers
    * Consumes:
    *  - dig: digit to write to 7 segment
    *  - center: boolean flag to indicate if colon light should be on
    */
    
    int bdig; // Variable for containing all LEDs in binary form
    
    // Checks which digit has been given and assigns appropriate binary list
    switch (dig) {
        case '0':
            bdig = 0b00000011;
            break;
        case '1':
            bdig = 0b10011111;
            break;
        case '2':
            bdig = 0b00100101;
            break;
        case '3':
            bdig = 0b00001101;
            break;
        case '4':
            bdig = 0b10011001;
            break;
        case '5':
            bdig = 0b01001001;
            break;
        case '6':
            bdig = 0b01000001;
            break;
        case '7':
            bdig = 0b00011111;
            break;    
        case '8':
            bdig = 0b00000001;
            break;  
        case '9':
            bdig = 0b00011001;
            break;
        case '-':
            bdig = 0b11111101;
            break;
        case ' ':
            bdig = 0b11111111;
            break;
    }
    
    // Turns on colon light for middle digits
    if (colon_on) {
        bdig--; // changes trailing 1 to 0
    }
    
    return bdig;
}

void write_dig(int bdig) {
    /*
    * Writes a single digit to the 7 segment by shifting it into registers
    * Consumes:
    *  - dig: digit to write to 7 segment
    *  - center: boolean flag to indicate if colon light should be on
    */
    
    int led_bin = 0; // Individual LED values
    
    // Writes digits in binary number to 7-segment, one LED at a time
    for (int i = 0; i < 8; i++) {
        
        // Extracts each digit from binary number
        led_bin = bdig % 2;
        bdig = bdig / 2;
        
        // Write to shift register, individual LEDs of 7 segment
        LATBbits.LATB14 = led_bin;

        // Enter bit with clock
        LATBbits.LATB15 = 1;
        LATBbits.LATB15 = 0;
    }
    
    return;
}

void update_digs(int number, int colon_on) { 

    char temp[5];
    char number_str[] = "    "; // 4 spaces
    int start_pos;
    
    if (number < 0) {
        snprintf(temp, sizeof(temp), "-%d", abs(number));
    } else {
        snprintf(temp, sizeof(temp), "%d", number);
    }
    
    start_pos = strlen(number_str) - strlen(temp);
    strncpy(number_str + start_pos, temp, strlen(temp));
    
    for (int i = 0; i < 4; i++) {
        // if colon requested, apply it to middle two positions
        if (i == 1 || i == 2) {
            digs[3 - i] = convert_to_bin(number_str[i], colon_on);
        }
        else {
            digs[3 - i] = convert_to_bin(number_str[i], 0);
        }
        
    }
    
    return;
}

void flash_seven_seg() {
    
    char number_str[] = "123456789-";

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            digs[j] = convert_to_bin(number_str[i], 0);
        }
        delay(750000);
    }
}


