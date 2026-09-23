#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "hcsr04.h"
#include "clock.h"
#include "motor.h"
#include "uart.h"
#include "LCD.h"



#define l_button BIT1
#define r_button BIT2
#define red BIT0
#define green BIT7 //9.7

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; //unlock outputs	
	PMMCTL0 = PMMPW; // Open PMM Module
	__enable_interrupt();

	p2init();//hcsr04
	motorpwm_init();
	TA3ClockInit();
	SETUP_UART_PINS(); //4.2 = TX, 4.3 = RX (UART0)
	INITIALIZE_UART();
	
	P9DIR |= green;
	uint8_t i;
	uint16_t num = 1;
	while(1){
		
		//enum parsing_state{AA1, AA2, }

		for(i = 0; i < 16; i++){
			num = num << i;
			showChar(i, 0);
			showChar(num,32);
			delay1ms(1000);
		}
			
	}

}
