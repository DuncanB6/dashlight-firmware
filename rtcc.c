/* 
 * File:   rtcc.c
 * Author: wapta
 *
 * Created on February 10, 2024, 1:52 PM
 */

#ifndef _rtcc_C_
#define _rtcc_C_

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include "p32xxxx.h"
#include <sys/attribs.h>

#include "rtcc.h"

#endif

struct rtccTime read_rtcc(void) {
    /*
    Parses the time and date from the RTCC
    Produces:
       - rtccTime: struct containing all individual numbers of date and time
    */
    
    struct rtccTime datetime;
    uint32_t time = RTCTIME;
    uint32_t date = RTCDATE;
    
    // parses 32 bit time integer into individual values (see datasheet)
    // 00000000 00000000 00000000 00000000
    // hour     minute   second   unused
    datetime.sec01 = (time >> 8) & 0b1111;
    datetime.sec10 = (time >> 12) & 0b111;
    datetime.min01 = (time >> 16) & 0b1111;
    datetime.min10 = (time >> 20) & 0b111;
    datetime.hr01 = (time >> 24) & 0b1111;
    datetime.hr10 = (time >> 28) & 0b11;
    
    // parses 32 bit date integer into individual values (see datasheet)
    // 00000000 00000000 00000000 00000000
    // year     month    day      week   
    datetime.wk = date & 0b111;
    datetime.dy01 = (date >> 8) & 0b1111;
    datetime.dy10 = (date >> 12) & 0b11;
    datetime.mn01 = (date >> 16) & 0b1111;
    datetime.mn10 = (date >> 20) & 0b1;
    datetime.yr01 = (date >> 24) & 0b1111;
    datetime.yr10 = (date >> 28) & 0b1111;
    
    return datetime;
}

