
#ifndef _CONFIG_H_
#define _CONFIG_H_

#pragma config JTAGEN = OFF
#pragma config FWDTEN   = OFF    // Turn off watchdog timer
#pragma config FNOSC    = FRCPLL // Select 8 MHz internal Fast RC (FRC) oscillator with PLL
#pragma config FPLLIDIV = DIV_2  // Divide PLL input (FRC) to get into required range [4 MHz, 5 MHz]
#pragma config FPLLMUL  = MUL_15 // PLL Multiplier
#pragma config FPLLODIV = DIV_1  // PLL Output Divider
#pragma config FPBDIV   = DIV_1  // Peripheral Clock divisor
#pragma config FSOSCEN = ON      // Turn on secondary oscillator

// Operating at 8 / 2 * 15 / 1 = 60 MHz

#endif
