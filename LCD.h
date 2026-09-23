#ifndef LCD_H
#define LCD_H


#include <msp430.h>
#include <stdint.h>


extern uint8_t Pos[6];
void scrollText(const char *text, uint16_t delay_ms);
void printChar(char c, uint8_t p);
void LCDinit(void);
void clearLCD(void);



#endif

