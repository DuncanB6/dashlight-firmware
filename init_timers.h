
#ifndef _setup_H_
#define _setup_H_

extern void ISRSetup (void);
extern void RTCCSetup (void);
extern void PINSetup(void);

#define CORE_TICKS 100000 // determines how often the 7 seg displays
#define _CORE_TIMER_VECTOR 0

#endif

