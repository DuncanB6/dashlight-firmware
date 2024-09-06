
/*
 * Duncan Boyd, Feb 10, 2024, duncan@wapta.ca
 * Purpose: Run a uc that can show some interesting metrics in my car through a
 *          7 segment display.
 */

#ifndef _main_C_
#define _main_C_

#include "xc.h"
#include "p32xxxx.h"
#include <stdio.h>
#include <sys/attribs.h>
#include <stdlib.h>
#include <string.h>

#include "CONFIG.h"
#include "rtcc.h"
#include "init_timers.h"
#include "sevenSegment.h"
#include "carInterface.h"

#endif

void delay(int time) {
    for (int i = 0; i < time; i += 1);
}

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
    write_dig(digs[digit_index]); 
        
    _CP0_SET_COUNT(0); // Set core count to 0
    _CP0_SET_COMPARE(CORE_TICKS); // Watches core count until next interrupt
}

int sample_adc(void) {
        
    AD1CON1bits.ADON = 1; // turn on ADC
    AD1CHSbits.CH0SA0 = 0; // select pin AN0
    AD1CON1bits.SAMP = 1; // start sampling
    
    for (int i = 0; i < 100; i++); // collect analog data for a bit
    
    AD1CON1bits.SAMP = 0;
    
    while (!AD1CON1bits.DONE);
    
    return ADC1BUF0;
}

int check_engine_status(void) {
    
    char message[100];
    int integer_data[4] = {0};
    
    sprintf(message, "010C\r");
    retrieve_data(message, integer_data);
    
    // confirm that we can receive data
    if (integer_data[0] != 65 || integer_data[1] != 12) {
        return 0;
    }
   
    return 1;
}

int main() {
    
    PINSetup(); // initializes pins
    
    ISRSetup(); // initialize ISR for the 7 seg updates
    
    flash_seven_seg(); // flashes numbers on 7 seg
    
    RTCCSetup(); // initialize the Real Time Clock and Calendar
    
    //UARTSetup(); // initialize the UART connection with the car
        
    struct rtccTime time;
    int formatted_time;
    int status = 0;
    char message[100];
    int integer_data[4];
    int rpm = 0;
    int speed = 0;
    int temperature = 40;
    int maf = 0;
    int mpg = 0;
        
    while(1) {
        
        status = 4 - (sample_adc() / 205); // range is 0-1023, assumes 5 settings
        
        if (status != 0) {
            if (!check_engine_status()) {
                // set screen to '----' while searching
                for (int i = 0; i < 4; i++) {
                    digs[i] = convert_to_bin('-', 0);
                }
                UARTSetup();
                continue;
            }
        }
        
        switch (status) {
            
            case 0: // time since start
                time = read_rtcc();
                if (time.hr01 != 0) {
                    formatted_time = time.hr10*1000 + time.hr01*100 + time.min10*10 + time.min01;
                }
                else {
                    formatted_time = time.min10*1000 + time.min01*100 + time.sec10*10 + time.sec01;
                }
                
                update_digs(formatted_time, 1);
                
                break;
                
            case 1: // get RPM
                sprintf(message, "010C\r");
                retrieve_data(message, integer_data);
                
                rpm = (256 * integer_data[2] + integer_data[3]) / 4;
                
                update_digs(rpm, 0);

                break;
                
            case 2: // intake air temp
                sprintf(message, "010F\r");
                retrieve_data(message, integer_data);
                
                temperature = integer_data[2] - 40;
                
                update_digs(temperature, 0);

                break;
                
            case 3: // speed
                sprintf(message, "010D\r");
                retrieve_data(message, integer_data);
                
                speed = integer_data[2];
                
                update_digs(speed, 0);

                break;
                
            case 4: // mpg
                sprintf(message, "010D\r");
                retrieve_data(message, integer_data);
                
                speed = integer_data[2];
                
                sprintf(message, "0110\r");
                retrieve_data(message, integer_data);
                
                maf = (256 * integer_data[2] + integer_data[3]) / 100;
                
                // protects against divide by 0 
                if (maf == 0) {
                    break;
                }
                
                mpg = speed * 7.718 / maf; // source: https://stackoverflow.com/questions/17170646/what-is-the-best-way-to-get-fuel-consumption-mpg-using-obd2-parameters
                
                update_digs(mpg, 0);

                break;
        }
        
        // add a delay unless we're on clock mode
        if (status != 0) {
            delay(1000);
        }
     
    }
    
    return 0;
}