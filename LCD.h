#ifndef LCD_H
#define LCD_H


#include <msp430.h>
#include <stdint.h>

void scrollText(const char *text, uint16_t delay_ms);
void showChar(char c, uint16_t position);
void LCDinit(void);


#endif

