#include <msp430.h>
#include <stdint.h>
#include "clock.h"
#include "uart.h"

volatile uint8_t rchar = 0;
volatile bool txfull = 0;

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
  txfull = false; //TX is not full
  
}

void SEND_CHAR_UART(unsigned char data) { // ASCII Character -> parse ASCII(1,2,3,4)
  // wait for UART port if busy
  while (txfull);
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
      UCA0IFG &= ~UCTXIFG;
    }
    
  }

