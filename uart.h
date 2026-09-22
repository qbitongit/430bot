#ifndef UART_H
#define UART_H
#include <stdint.h>

extern volatile uint8_t rchar;
extern volatile bool txfull;


void SETUP_UART_PINS(void);
void INITIALIZE_UART(void);

void SEND_CHAR_UART(unsigned char data);
void SEND_STRING_UART(const char *data);
void SEND_INTEGER_UART(uint32_t num);
uint32_t RECEIVE_UART (void);


/*
 * UART baud-rate selection from an 8 MHz SMCLK, oversampled; values from TI
 * SLAU367 Table 30-5.  9600 by default, with 19200, 38400, 57600 and 115200
 * selectable at build time through UART_BAUD.
 */
 
#define UART_CLK_SEL     UCSSEL__SMCLK

#ifndef UART_BAUD
#define UART_BAUD        9600
#endif

#if   UART_BAUD == 9600
/*  N = 8000000/9600 = 833.33  -> BRW=52, BRF=1, BRS=0x49 */
#define UART_BRW         52
#define UART_MCTLW       ((0x49 << 8) | UCOS16 | (0x01 << 4))

#elif UART_BAUD == 19200
/*  N = 8000000/19200 = 416.67 -> BRW=26, BRF=0, BRS=0xB6 */
#define UART_BRW         26
#define UART_MCTLW       ((0xB6 << 8) | UCOS16 | (0x00 << 4))

#elif UART_BAUD == 38400
/*  N = 8000000/38400 = 208.33 -> BRW=13, BRF=0, BRS=0x84 */
#define UART_BRW         13
#define UART_MCTLW       ((0x84 << 8) | UCOS16 | (0x00 << 4))

#elif UART_BAUD == 57600
/*  N = 8000000/57600 = 138.89 -> BRW=8, BRF=10, BRS=0xF7 */
#define UART_BRW         8
#define UART_MCTLW       ((0xF7 << 8) | UCOS16 | (0x0A << 4))

#elif UART_BAUD == 115200
/*  N = 8000000/115200 = 69.44 -> BRW=4, BRF=5, BRS=0x55 */
#define UART_BRW         4
#define UART_MCTLW       ((0x55 << 8) | UCOS16 | (0x05 << 4))

#else
#error "Unsupported UART_BAUD (use 9600/19200/38400/57600/115200)"
#endif

#endif

