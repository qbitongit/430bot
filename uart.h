#ifndef UART_H
#define UART_H
#include <stdint.h>

extern volatile uint8_t rchar;
extern volatile bool txfull;


void SETUP_UART_PINS(void);
void INITIALIZE_UART(void);

void SEND_CHAR_UART(unsigned char data);
void set_baud_9600(void);
void set_baud_57600(void);
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
#define UART_BAUD        57600
#endif

#if UART_BAUD == 9600

/* N = 1000000/9600 = 104.17
 * UCOS16 = 1
 * BRW = 6, BRF = 8, BRS = 0x20
 */
#define UART_BRW         6
#define UART_MCTLW       ((0x20 << 8) | UCOS16 | (0x08 << 4))

#elif UART_BAUD == 19200

/* N = 1000000/19200 = 52.08
 * UCOS16 = 1
 * BRW = 3, BRF = 4, BRS = 0x02
 */
#define UART_BRW         3
#define UART_MCTLW       ((0x02 << 8) | UCOS16 | (0x04 << 4))

#elif UART_BAUD == 38400

/* N = 1000000/38400 = 26.04
 * UCOS16 = 1
 * BRW = 1, BRF = 10, BRS = 0x00
 */
#define UART_BRW         1
#define UART_MCTLW       ((0x00 << 8) | UCOS16 | (0x0A << 4))

#elif UART_BAUD == 57600

/* N = 16000000/57600 = 277.7777
 * UCOS16 = 1 --> 17.361
 * BRW = 17
 BRS = 0x4A --> 0.3575
 */
 
#define UART_BRW    17
#define UART_MCTLW (UCOS16 | (0x4A << 8))


#elif UART_BAUD == 115200

/* N = 1000000/115200 = 8.68
 * UCOS16 = 0
 * BRW = 8, BRS = 0xD6
 */
#define UART_BRW         8
#define UART_MCTLW       (0xD6 << 8)


#else
#error "Unsupported UART_BAUD (use 9600/19200/38400/57600/115200)"
#endif

#endif

