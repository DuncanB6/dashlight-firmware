
#ifndef _carInterface_H_
#define _carInterface_H_

#endif

extern void parse_car_data(char *message, int *integer_data);
extern void UARTSetup(void);
extern void read_uart(char *response);
extern void wait_for_obd(void);
extern void retrieve_data(char *message, int *integer_data);