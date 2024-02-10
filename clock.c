
#include "CONFIG.h"
#include <sys/attribs.h>

#define CORE_TICKS 100000 // determines how often the 7 seg displays
#define _CORE_TIMER_VECTOR 0

char digs[] = {7, 8, 3, 2}; // digits to be written to 7 seg
int digit_index = 0; // index for 7 seg interrupt, cycles through digits

// Provides a delay
void delay(int time) {
    int i;
    for (i = 0; i < time; i += 1);
}

// Writes a single digit to the 7 segment
void write_dig(int dig, int center) {
    
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
        bdig--;
    }
    
    int led_bin = 0; // Individual LED values
    
    // Writes digits in binary number to 7-segment
    for (int i = 0; i < 8; i++) {
        
        // Extracts each digit from binary number
        led_bin = bdig % 2;
        bdig = bdig / 2;
        
        // Write to shift register, individual LEDs
        LATBbits.LATB14 = led_bin;

        // Enter bit with clock
        LATBbits.LATB15 = 1;
        LATBbits.LATB15 = 0;
    }
    
    return;
}

// Updates 7 seg digits given a number between 0 and 9999
int update_digs(int number) {
    
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

// flashes all number on 7 seg as an initialization routine
void init_sevenseg() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            digs[j] = i;
        }
        delay(750000);
    }
    
}

// ISR that triggers every 1 second
void __ISR(_CORE_TIMER_VECTOR, IPL6SOFT) CoreTimerISR(void) {
    
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
    
    // provides an indicator for the middle two digits (for colon)
    int c = digit_index%3; 
   
    // i goes up to 4 to stagger the digit this function leaves off on
    write_dig(digs[digit_index], c); 
        
    _CP0_SET_COUNT(0); // Set core count to 0
    _CP0_SET_COMPARE(CORE_TICKS); // Watches core count until next interrupt
}

// Initializes pins and interrupts
void setup() {
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

    // Initializes core interrupt
    __builtin_disable_interrupts(); // Disables interrupts while working on them
    _CP0_SET_COMPARE(CORE_TICKS); // Starts watching core timer
    IPC0bits.CTIP = 6; // Interrupt priority
    IPC0bits.CTIS = 0; // Interrupt sub-priority
    IFS0bits.CTIF = 0; // Interrupt flag
    IEC0bits.CTIE = 1; // Interrupt enable
    __builtin_enable_interrupts(); // Enables interrupts
    _CP0_SET_COUNT(0); // Sets core timer to 0
    
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
    RTCTIME = time;
    RTCDATE = time;
    RTCCONbits.ON = 1;
    
    
    
    
}

int main() {
    
    setup(); // Initializes pins, interrupts, RTCC
    
    init_sevenseg(); // Flashes numbers on 7 seg
    
    int randum = 0;
    uint32_t time;
    uint8_t seconds;
    while(1) {
        // 00000000 00000000 00000000 00000000
        // hour     minute   second   unused
        time = RTCTIME;
        seconds = (time >> 8) & 0b00001111;
        randum = randum + 10;
        if (!update_digs(seconds)) {
            init_sevenseg();
        }
        
    }
    
    return 0;
}