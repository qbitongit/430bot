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

int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; //unlock outputs	
	PMMCTL0 = PMMPW; // Open PMM Module
	__enable_interrupt();

	p2init();//hcsr04
	motorpwm_init();
	TA3ClockInit();
	SETUP_UART_PINS(); //4.2 = TX, 4.3 = RX (UART0)
	INITIALIZE_UART();
	LCDinit();


	//red/left led init
	P1DIR |= red;
	

/*


	uint32_t sum;
	uint16_t avg

	while(1){

		
				
		find avg of signal every second:
		for(65563)
			parse rchar
			if rchar == eeg
				sum += rchar
		
		
		avg=sum >> 16;

		print on lcd in hex

		delay
		
		
		
	}			

*/


//talking to HC-05 in AT mode

	while(1){

		SEND_STRING_UART("HELLO\n");
	
		
	}



}
