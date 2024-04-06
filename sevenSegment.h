
#ifndef _sevenSegment_H_
#define _sevenSegment_H_

extern void write_dig(int dig, int center);
extern int update_digs(int number);
extern void init_sevenseg();
//extern void __ISR(_CORE_TIMER_VECTOR, IPL6SOFT) CoreTimerISR(void);

// For 7 seg interrupt
//extern char digs[] = {7, 8, 3, 2}; // Digits to be written to 7 seg
//extern int digit_index = 0; // Index for 7 seg digit, cycles through digits

#define CORE_TICKS 100000 // determines how often the 7 seg displays
#define _CORE_TIMER_VECTOR 0

// For 7 seg interrupt
extern char digs[]; // Digits to be written to 7 seg
extern int digit_index; // Index for 7 seg digit, cycles through digits

#endif

