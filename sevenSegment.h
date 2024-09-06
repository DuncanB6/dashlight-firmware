
#ifndef _sevenSegment_H_
#define _sevenSegment_H_

extern void write_dig(int dig);
extern void update_digs(int number, int colon_on);
extern void flash_seven_seg();
extern uint8_t convert_to_bin(char dig, int colon_on);

#define CORE_TICKS 100000 // determines how often the 7 seg displays
#define _CORE_TIMER_VECTOR 0

// For 7 seg interrupt
extern uint8_t digs[4]; // Digits to be written to 7 seg
extern int digit_index; // Index for 7 seg digit, cycles through digits

#endif

