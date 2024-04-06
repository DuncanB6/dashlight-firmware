
#ifndef _rtcc_H_
#define _rtcc_H_

typedef struct rtccTime{
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
};


struct rtccTime read_rtcc(void);

#endif