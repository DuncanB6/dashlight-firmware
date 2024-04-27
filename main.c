
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

int main() {
    
    PINSetup(); // Initializes pins, interrupts, RTCC
    
    ISRSetup(); // Initialize ISR for the 7 seg updates
    
    RTCCSetup(); // Initialize the Real Time Clock and Calendar
    
    init_sevenseg(); // Flashes numbers on 7 seg
  
    RPB3Rbits.RPB3R = 0b0001; // pin B3 set to TX
    U1RXRbits.U1RXR = 0b0100; // pin B2 set to RX
    U1MODEbits.BRGH = 0;
    U1BRG = ((30000000 / 9600) / 8) - 1; // baud rate
    U1MODEbits.PDSEL = 0b00; // parity bits (0 = 8 bit, no parity)
    U1MODEbits.STSEL = 0; // stop bits (0 = 1 stop bit)
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
   
    
    struct rtccTime time;
    uint16_t formatted_time;
    int status = 0;
    char message[100];
    int i = 0;
    int j = 0;
    sprintf(message, "test");
    while(1) {
        
        write_uart(message);
        
        i++;
        
        j = 0;
        while (j < 5000000) {
            j++;
        }
        read_uart(message, 100);

        
        
        switch (status) {
            case 0: // time since start
                time = read_rtcc();
                if (time.hr01 != 0) {
                    formatted_time = time.hr10*1000 + time.hr01*100 + time.min10*10 + time.min01;
                }
                else {
                    formatted_time = time.min10*1000 + time.min01*100 + time.sec10*10 + time.sec01;
                }
                if (!update_digs(formatted_time)) {
                    init_sevenseg();
                }
        }
     
    }
    
    return 0;
}