/* 
 * File:   setup.c
 * Author: wapta
 *
 * Created on February 10, 2024, 1:54 PM
 */

#ifndef _setup_C_
#define _setup_C_

#include "xc.h"
#include "p32xxxx.h"
#include <stdio.h>
#include <sys/attribs.h>
#include <stdlib.h>

//#include "main.h"
//#include "sevenSegment.h"
#include "init_timers.h"

#endif

void ISRSetup () {
    // Initializes core interrupt
    __builtin_disable_interrupts(); // Disables interrupts while working on them
    _CP0_SET_COMPARE(CORE_TICKS); // Starts watching core timer
    IPC0bits.CTIP = 6; // Interrupt priority
    IPC0bits.CTIS = 0; // Interrupt sub-priority
    IFS0bits.CTIF = 0; // Interrupt flag
    IEC0bits.CTIE = 1; // Interrupt enable
    __builtin_enable_interrupts(); // Enables interrupts
    _CP0_SET_COUNT(0); // Sets core timer to 0
}

void RTCCSetup () {
    // Turn on the oscillator for RTCC
    OSCCONbits.SOSCEN = 1;
    while(!OSCCONbits.SOSCRDY);
    
    // Allow the RTCC values to be writable
    SYSKEY = 0x0;
    SYSKEY = 0xaa996655;
    SYSKEY = 0x556699aa;
    RTCCONbits.RTCWREN = 1;
    
    // Write a time and date of 0 to the RTCC, turn it on
    RTCCONbits.ON = 0;
    uint32_t time = 0;
    uint32_t date = 0;
    RTCTIME = time;
    RTCDATE = date;
    RTCCONbits.ON = 1;
}

// Initializes pins and interrupts
void PINSetup() {
    ANSELBbits.ANSB3 = 0; // RB3 is digital (LED))
    ANSELBbits.ANSB12 = 0; // RB12 is digital (Shift input, digits)
    ANSELBbits.ANSB13 = 0; // RB13 is digital (Shift clock, digits)
    ANSELBbits.ANSB14 = 0; // RB14 is digital (Shift input, indiv LEDs)
    ANSELBbits.ANSB15 = 0; // RB15 is digital (Shift clock, indiv LEDs)
    TRISBbits.TRISB3 = 0; // RB3 is output (LED)
    TRISBbits.TRISB12 = 0; // RB12 is output (Shift input, digits)
    TRISBbits.TRISB13 = 0; // RB13 is output (Shift clock, digits)
    TRISBbits.TRISB14 = 0; // RB14 is output (Shift input, indiv LEDs)
    TRISBbits.TRISB15 = 0; // RB15 is output (Shift clock, indiv LEDs)
}

