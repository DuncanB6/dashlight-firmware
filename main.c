
/*
 * Duncan Boyd, Feb 10, 2024, duncan@wapta.ca
 * Purpose: Run a uc that can show some interesting metrics in my car through a
 * 7 segment display.
 */

#ifndef _main_C_
#define _main_C_

#include "xc.h"
#include "p32xxxx.h"
#include <stdio.h>
#include <sys/attribs.h>
#include <stdlib.h>

#include "CONFIG.h"
#include "rtcc.h"
#include "init_timers.h"
#include "sevenSegment.h"

#endif

void delay(int time) {
    /*
    * Provides a delay by occupying the uc
    */
    int i;
    for (i = 0; i < time; i += 1);
}

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

int main() {
    
    PINSetup(); // Initializes pins, interrupts, RTCC
    
    ISRSetup(); // Initialize ISR for the 7 seg updates
    
    RTCCSetup(); // Initialize the Real Time Clock and Calendar
    
    init_sevenseg(); // Flashes numbers on 7 seg
    
    uint32_t time;
    uint8_t seconds;
    while(1) {
        time = RTCTIME;
        seconds = (time >> 8) & 0b00001111;
        if (!update_digs(seconds)) {
            init_sevenseg();
        }
    }
    
    return 0;
}