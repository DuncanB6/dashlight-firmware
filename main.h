

#ifndef RTCC_H__
#define RTCC_H__

#include "xc.h"
#include "p32xxxx.h"
#include <stdio.h>

#pragma config JTAGEN = OFF
#pragma config FWDTEN   = OFF    // Turn off watchdog timer
#pragma config FNOSC    = FRCPLL // Select 8 MHz internal Fast RC (FRC) oscillator with PLL
#pragma config FPLLIDIV = DIV_2  // Divide PLL input (FRC) to get into required range [4 MHz, 5 MHz]
#pragma config FPLLMUL  = MUL_15 // PLL Multiplier
#pragma config FPLLODIV = DIV_1  // PLL Output Divider
#pragma config FPBDIV   = DIV_1  // Peripheral Clock divisor
#pragma config FSOSCEN = ON      // Turn on secondary oscillator

// Operating at 8 / 2 * 15 / 1 = 60 MHz

typedef struct {
    unsigned char sec01;
    unsigned char sec10;
    unsigned char min01;
    unsigned char min10;
    unsigned char hr01;
    unsigned char hr10;
    
    unsigned char yr01;
    unsigned char yr10;
    unsigned char mn01;
    unsigned char mn10;
    unsigned char dy01;
    unsigned char dy10;
    unsigned char wk;
} rtccTime;

rtccTime readRTCC();


#endif
