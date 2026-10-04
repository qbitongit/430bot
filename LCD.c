#include <msp430.h>
#include "LCD.h"
#include "clock.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// The Position accounts for Offset of Alphanumeric character positions in the LCD Memory Map
extern uint8_t Pos[6] = {9, 5, 3, 18, 14, 7};          // Define and populate Position, Range 0 - 5

void showChar(char c, uint8_t p); //prototyping

void scrollText(const char *text, uint16_t delay) {
    uint8_t textLen = strlen(text);
    uint8_t scrollPos = 0;
    uint8_t i;

    while (1) {
        for (i = 0; i < 6; i++) {
            showChar(text[(scrollPos + i) % textLen], Pos[i]);
        }

        scrollPos = (scrollPos + 1) % textLen;
        delay1ms(delay); // Delay in microseconds
    }
}

void clearLCD(void){
    uint8_t i;
    for(i = 6; i >= 1; i--){
        LCDMEM[Pos[i]-1] = 0x00;
        LCDMEM[Pos[i]] = 0x00;
    }

    
}

void printAsHex(uint16_t num){
    clearLCD();
    showChar('0', 0);
    showChar('x', 1);
    uint8_t i;
    for(i = 0; i<4; i++){
      showChar(i+2, ((char)(num & (0x0F<<i))));
    }
}

// *************************************************************************************************
// **  Show Character at LCD Position, works with Space, Numbers, Upper and Lower case Characters **
// *************************************************************************************************
void printChar(char c, uint8_t p)
{
// Space/Number Characters:  [  SP   ]  [   !   ]  [   "   ]  [   #   ]  [   $   ]  [   %   ]  [   &   ]  [   '   ]  [   (   ]  [   )   ]  [   *   ]  [   +   ]  [   ,   ]
const char sp_num[26][2]  = {0x00,0x00, 0x60,0x01, 0x40,0x40, 0x61,0x50, 0xB7,0x50, 0x00,0x29, 0x10,0xCA, 0x00,0x20, 0x00,0x22, 0x00,0x88, 0x03,0xAA, 0x03,0x50, 0x00,0x08,
                             0x03,0x00, 0x00,0x01, 0x00,0x28, 0xFC,0x28, 0x60,0x20, 0xDB,0x00, 0xF3,0x00, 0x67,0x00, 0xB7,0x00, 0xBF,0x00, 0xE4,0x00, 0xFF,0x00, 0xF7,0x00};
// Space/Number Characters:  [   -   ]  [   .   ]  [   /   ]  [   0   ]  [   1   ]  [   2   ]  [   3   ]  [   4   ]  [   5   ]  [   6   ]  [   7   ]  [   8   ]  [   9   ]

// Upper Case Characters:    [   A   ]  [   B   ]  [   C   ]  [   D   ]  [   E   ]  [   F   ]  [   G   ]  [   H   ]  [   I   ]  [   J   ]  [   K   ]  [   L   ]  [   M   ]
const char  upper[26][2]  = {0xEF,0x00, 0xF1,0x50, 0x9C,0x00, 0xF0,0x50, 0x9F,0x00, 0x8F,0x00, 0xBD,0x00, 0x6F,0x00, 0x90,0x50, 0x78,0x00, 0x0E,0x22, 0x1C,0x00, 0x6C,0xA0,
                             0x6C,0x82, 0xFC,0x00, 0xCF,0x00, 0xFC,0x02, 0xCF,0x02, 0xB7,0x00, 0x80,0x50, 0x7C,0x00, 0x0C,0x28, 0x6C,0x0A, 0x00,0xAA, 0x00,0xB0, 0x90,0x28};
// Upper Case Characters:    [   N   ]  [   O   ]  [   P   ]  [   Q   ]  [   R   ]  [   S   ]  [   T   ]  [   U   ]  [   V   ]  [   W   ]  [   X   ]  [   Y   ]  [   Z   ]

// Lower Case Characters:    [   a   ]  [   b   ]  [   c   ]  [   d   ]  [   e   ]  [   f   ]  [   g   ]  [   h   ]  [   i   ]  [   J   ]  [   k   ]  [   l   ]  [   m   ]
const char  lower[26][2]  = {0x21,0x12, 0x3F,0x00, 0x1B,0x00, 0x7B,0x00, 0x1A,0x08, 0x0E,0x00, 0xF7,0x00, 0x2F,0x00, 0x10,0x10, 0x78,0x00, 0x00,0x72, 0x0C,0x00, 0x2B,0x10,
                             0x21,0x10, 0x0A,0x18, 0xCF,0x00, 0xE7,0x00, 0x0A,0x00, 0x11,0x02, 0x03,0x10, 0x38,0x00, 0x08,0x08, 0x28,0x0A, 0x00,0xAA, 0x00,0xA8, 0x12,0x08};
// Lower Case Characters:    [   n   ]  [   o   ]  [      ]  [   q   ]  [   r   ]  [   s   ]  [   t   ]  [   u   ]  [   v   ]  [   w   ]  [   x   ]  [   y   ]  [   z   ]

    if (c >= ' ' && c <= '9')
      {
        // Display special printable characters & Numbers
        LCDMEM[Pos[p]]   =  sp_num[c-32][0];
        LCDMEM[Pos[p]+1] =  sp_num[c-32][1];
      }
    else if (c >= 'A' && c <= 'Z')
      {
        // Display Upper-case Letters
        LCDMEM[Pos[p]]   = upper[c-65][0];
        LCDMEM[Pos[p]+1] = upper[c-65][1];
      }
    else if (c >= 'a' && c <= 'z')
      {
        // Display lower-case letters
        LCDMEM[Pos[p]]   = lower[c-97][0];
        LCDMEM[Pos[p]+1] = lower[c-97][1];
      }
    else
      {
        // Turn all segments on for other character
        LCDMEM[Pos[p]]   = 0xFF;
        LCDMEM[Pos[p]+1] = 0xFA;
      }
}

void LCDinit(void)
{
// Initialize LCD segments 0 - 21; 26 - 43
   LCDCPCTL0 = 0xFFFF;
   LCDCPCTL1 = 0xFC3F;
   LCDCPCTL2 = 0x00FF;

    PJSEL0 = BIT4 | BIT5;                      // Turn on LFXT (Low Frequency Crystal Oscillator)
// Configure LFXT 32kHz Crystal Oscillator
   CSCTL0_H = CSKEY >> 8;                      // Unlock CS registers
   CSCTL4 &= ~LFXTOFF;                         // Enable LFXT  (Low Frequency Crystal Oscillator)
         do
           {
            CSCTL5 &= ~LFXTOFFG;               // Clear LFXT fault flag
            SFRIFG1 &= ~OFIFG;
            }
            while (SFRIFG1 & OFIFG);           // Test oscillator fault flag
            CSCTL0_H = 0;                      // Lock CS registers

// Initialize LCD_Clock
// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
            LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

// VLCD generated internally,
// V2-V4 generated internally, V5 to ground
// Set VLCD voltage to 2.60V
// Enable charge pump and select internal reference for it
           LCDCVCTL     =   VLCD_1 | VLCDREF_0 | LCDCPEN;
           LCDCCPCTL    = LCDCPCLKSYNC;         // Clock synchronization enabled
           LCDCMEMCTL   = LCDCLRM;              // Clear LCD memory
// Turn LCD on
           LCDCCTL0 |= LCDON;
}

/* --COPYRIGHT--
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
