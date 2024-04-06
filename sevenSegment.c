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
char digs[] = {7, 8, 3, 2}; // Digits to be written to 7 seg
int digit_index = 0; // Index for 7 seg digit, cycles through digits

void write_dig(int dig, int center) {
    /*
    * Writes a single digit to the 7 segment by shifting it into registers
    * Consumes:
    *  - dig: digit to write to 7 segment
    *  - center: boolean flag to indicate if colon light should be on
    */
    
    int bdig = 0b00000000; // Variable for containing all LEDs in binary form
    
    // Checks which digit has been given and assigns appropriate binary list
    switch (dig) {
        case 0:
            bdig = 0b00000011;
            break;
        case 1:
            bdig = 0b10011111;
            break;
        case 2:
            bdig = 0b00100101;
            break;
        case 3:
            bdig = 0b00001101;
            break;
        case 4:
            bdig = 0b10011001;
            break;
        case 5:
            bdig = 0b01001001;
            break;
        case 6:
            bdig = 0b01000001;
            break;
        case 7:
            bdig = 0b00011111;
            break;    
        case 8:
            bdig = 0b00000001;
            break;  
        case 9:
            bdig = 0b00011001;
            break;
    }
    
    // Turns on colon light for middle digits
    if (center) {
        bdig--; // changes trailing 1 to 0
    }
    
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

int update_digs(int number) {
    /*
     Updates the readout on the 7 segment display
     Consumes:
        - number: a 4 digit number that will be displayed on 7 segment
     Returns:
        - an integer that confirms success, returns 0 on failure
     */
    
    if (number < 0 || number > 9999) {
        return 0;
    }
    
    for (int i = 0; i < 4; i++) {
        if (number > 0) {
            digs[i] = number%10;
            number = number/10;
        }
        else {
            digs[i] = 0;
        }
    }
    
    return 1;
}

void init_sevenseg() {
    /*
     Flashes numbers to 7 segment for aesthetic purposes
    */
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            digs[j] = i;
        }
        delay(750000);
    }
    
}
/*
// ISR that triggers every 1 second
void __ISR(_CORE_TIMER_VECTOR, IPL6SOFT) CoreTimerISR(void) {
    //Core ISR that updates the 7 segment display using global variables
    
    
    IFS0bits.CTIF = 0; // Clears CT interrupt flag
    
    // Cycles through index range
    digit_index++;
    if (digit_index >= 4) {
        digit_index = 0;
    }
    
    // Shifts one extra digit, as it's unused
    if (digit_index == 0) {
        LATBbits.LATB12 = 1;
    }
    else {
        LATBbits.LATB12 = 0;
    }

    // Enter bit with clock
    LATBbits.LATB13 = 1;
    LATBbits.LATB13 = 0;
       
    // i goes up to 4 to stagger the digit this function leaves off when it ends
    write_dig(digs[digit_index], digit_index%3); 
        
    _CP0_SET_COUNT(0); // Set core count to 0
    _CP0_SET_COMPARE(CORE_TICKS); // Watches core count until next interrupt
}
*/



