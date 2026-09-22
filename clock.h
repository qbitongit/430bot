#ifndef CLOCK_H
#define CLOCK_H
#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

void TA3ClockInit(void);
void delay1us(uint16_t num);
void delay1ms(uint16_t num);
void usTimerStart(void);
uint16_t usTimerCheck(void);

#endif
