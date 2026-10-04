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

int main(void) {
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
	//parsing fsm to get raw data and transmit thru bluetooth
	enum parseState{AA1, AA2, code, eeg_length, hiEEG, lowEEG, transmit} state, prevState;
	bool isNewState;
	state = AA1;
	prevState = transmit;
	uint16_t rawEEG;
	uint8_t stateTimer = 0;


	while(1){

		isNewState = !(state == prevState);

		switch(state){
		//AA1: checks if rchar == AA before moving to AA2
			case AA1:
				if(rchar == 0xAA){
					state = AA2;
				}
			break;

		//AA2: checks if rchar == AA then moves to waiting for code
		
			case AA2:
				if(rchar == 0xAA){
					state = code;
				}
				else{
					state = AA2;
				}
			break;
		//code: if rchar == 0x80 go to eeg_lenth
			case code:
				if(isNewState){
					stateTimer = 0;
				}
				while(stateTimer < 200){
					stateTimer++;
					if(rchar == 0x80){
						state = eeg_length;
					}
				}
				state = AA1;
			break;
			
		//eeg_length: if rchar == 0x02 move to eeg

			case eeg_length:
				if(isNewState){
					stateTimer = 0;
				}
				while(stateTimer < 200){
					stateTimer++;
					if(rchar == 0x02){
						state = hiEEG;
					}
				}
				state = AA1;
			break;

			case hiEEG:
				rawEEG |= (rchar << 8);
				state = lowEEG;
			break;

			case lowEEG:
				rawEEG |= rchar;
				state = transmit;
			break;

			case transmit:
				//write to LCD
				printAsHex(rawEEG);
				//set baud 9600 for HC-05
				set_baud_9600();
				SEND_INTEGER_UART(rawEEG);
				while(txfull);
				set_baud_57600();
				state = AA1;

			default:
				state = AA1;
			break;
				
		}
		
	}


*/
}
