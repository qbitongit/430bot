#include <msp430.h>
#include <stdint.h>
#include "clock.h"
#include "uart.h"




/*
    Configuration:
        Baud Rate: 57600
        Data width: 8-Bit data, no parity, 1 stop bit, LSB first, no flow
   control System Clock: SMCLK
*/

#define TX_BUFFER_SIZE 64
char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

#define RX_BUFFER_SIZE 64
char rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void SETUP_UART_PINS(void) {
  // 4.2 -> UCA0TXD ; P4.3 -> UCA0RXD

  // TX
  P4DIR |= BIT2;

  // RX
  P4DIR &= ~BIT3;

  // Selecting to TX & RX
  P4SEL0 |= BIT2 | BIT3;
  P4SEL1 &= ~(BIT2 | BIT3);

  //enable interrupts
  P4IE |= (BIT2 | BIT3);
  
};

void INITIALIZE_UART(void) {
  // UCA Control word 0
  // Some of these fields are ready set to 0 by default
  // but I added anyways to be more flexible in future applications

  // UCA0CTLW0 = UCSWRST; // Reset all fields (optional)
  UCA0CTLW0 |= UCSWRST; // enable reset state (this will allow us to edit the settings)
  UCA0CTLW0 &= ~UCPEN;        // 0 for ParityDisable, 1 for enable
  UCA0CTLW0 &= ~UCMSB;        // 0 for LSB first, 1 for MSB first
  UCA0CTLW0 |= UCSSEL__SMCLK; // Use SMCLK
  
  // UCA Baud Rate Control Word
  UCA0BRW = UART_BRW;

  // UCA Modulation Control Word Register
  
  UCA0MCTLW = UART_MCTLW;
  
  UCA0CTLW0 &= ~UCSWRST; // disable reset state

  UCA0IE = (UCRXIE|UCTXIE); // Interrupts for sending & receiving data

  // optional flush the read buffer
  volatile unsigned char dummy;
  dummy = UCA0RXBUF;   // Reading the buffer clears the RXIFG flag
  UCA0IFG &= ~UCRXIFG; // Manually ensure the flag is zeroed
  
}

void SEND_CHAR_UART(unsigned char data) { // ASCII Character -> parse ASCII(1,2,3,4)
  // wait for UART port if busy
  while (!(txfull));
  UCA0TXBUF = data;
  txfull = true;
}

void SEND_STRING_UART(const char *str) {
  while (*str) {
    SEND_CHAR_UART(*str++);
  }
}

void SEND_INTEGER_UART(uint32_t num) {
  char buf[10];
  int i = 0;

  if (num == 0) {
    SEND_CHAR_UART('0');
    return;
  }

  while (num > 0) {
    buf[i++] = (num % 10) + '0';
    num /= 10;
  }

  while (i > 0) {
    SEND_CHAR_UART(buf[--i]);
  }
}

uint32_t RECEIVE_UART(void) {
  // wait for reception
  usTimerStart();
  while(!(UCA0IFG & UCRXIFG)){
    if(usTimerCheck() > 100) break;
  }
  UCA0IFG &= ~UCRXIFG;
  return UCA0RXBUF;
  
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {

    if(UCA0IFG & UCRXIFG){
      rchar = (UCA0RXBUF & 0xFF);
    }
    if(UCA0IFG & UCTXIFG){
      txfull = false;
    }
    
  }



/*
    DELETE
    Steps to setup UART

    1. look for backchannel UART on board user's guide
        look for eUSCI (slau627a.pdf)
        for this MCU is eUSCI_A1
        Channel A (UART, SPI) is usally async while Channel B is synchronous
   (I2C, SPI)

    2. Pin Mapping
        look for the PIN mapping diagram on the datasheet and look for the
   corresponding TXD, RXD ; In this MCU the pin mapping are the following
        Pin 5.4/UCA0SIMO/UCA0TXD/S1288
        Pin 5.5/UCA0SOMI/UCA0RXD/S1
        Pin 3.4 / UCA0TXD
        Pin 3.5 / UCA0RXD

        in this case we have to sets of ports, we have to figure out which one
   to use go to the board user's guide and check the schematic of the jumpers
        page 33 and it say that the port is connected is P3.4 & P3.5

    3. Pin Functions divert
        look for Pin Functions table on the datasheet (for P3.4 & P3.5) Page 102
        and set the pin to the right pin function in this case UCA0TXD
        P3.4 P3SEL1 == 0 and P3SEL0 == 1 ( same for both pins )

    4. Find the recommended settings for UART
        You can manually calculate this, however, in the MCU user's guide they
   have recommended settings for typical baud rates table, look for this (P.
   589) For this case that we are using the the SMCLK (1MHz) and Baud Rate 9600
        The recommended settings are UCOS16=1, UCBRx=6, UCBRFx=8, UCBRSx=0x20

        Example of manual calculation
        (clk speed / baud rate) / oversampling
        (1000000/9600) = 104.16/16 -> 6.xx (This is why UCBR is 6)

    5. Adjust parameters for UART
        Go to family MCU user's guide and look for eUSCI_A UART Registers; this
   are also called configuration registers (Starts @ P.692 ) and look for all
   the settings that you need to setup


    6. Polling the Flags
        UCA0IFG // Register
        UCTXIFG // TX flag -> 0 busy, 1 ready to tx
        UCRXIFG // RX flag -> 0 no new data, 1 new data received
        UCA0TXBUF // TX buffer  // writing to TX buffer start transmission auto
   (TX Flag goes to zero) UCA0RXBUF // RX buffer // reading the RX buffer clear
   the RX flag automatically
*/
