#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "motor.h"

#define stby BIT5 //1.5 enable

#define Bin1 BIT6 //1.6, 1.7 A motor (TA0.1 & 1.1)
#define Bin2 BIT7 

#define Ain BIT3 //3.3, 1.3 B motor (TA1.1 & 1.2)


void motorpwm_init (void){

	//b motor TA0 init


	TA0CTL = (TASSEL_2 | MC_0); //smclk, stop
	TA0CCR0 = 60000; //maxcount
	TA0CCTL1 = OUTMOD_7; //set pwm to set/reset
	TA0CCTL2 = OUTMOD_7; //set pwm to set/reset

    //b motor pwm pin init
	P1DIR |= (Bin1|Bin2); //set p1.6 & 1.7 as pwm output pins
	P1SEL0 |= (Bin1|Bin2);
	P1SEL1 |= (Bin1|Bin2);
	P1SELC &= ~(Bin1|Bin2);

	//en
	P1DIR |= stby;
	P1OUT |= stby; //enables motors


	//a motor
	//ta1 init


	TA1CTL = (TASSEL_2 | MC_0); //smclk,up
	TA1CCR0 = 60000; //maxcount
	TA1CCTL1 = OUTMOD_7; //set pwm to set/reset
	TA1CCTL2 = OUTMOD_7; //set pwm to set/reset

	//a motor pwm pin init
	P3DIR |= Ain; 
	P1DIR |= Ain;
	P3SEL0 &= ~Ain;
	P1SEL0 |= Ain;
	P3SEL1 |= Ain;
	P1SEL1 &= ~Ain;
	P3SELC &= ~Ain;
	P1SELC &= ~Ain;

}

void p_bMotFwd (uint16_t duty){

	TA0CCR2 = 0; //set CCR2/Bin2 on @ 0
	TA0CCR1 = duty;//set CCR1/Bin1 on @ duty
	TA0CTL &= ~MC1;
	TA0CTL |= MC0;

}

void p_bMotBkwd (uint16_t duty){

	TA0CCR2 = duty;//set Bin2 on at duty
	TA0CCR1 = 0; //set CCR1/Bin1 to 0
	TA0CTL &= ~MC1;
	TA0CTL |= MC0;

}

void p_bMotCoast (void){

	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CTL |= MC0;
	TA1CTL &= ~MC1;

}


void p_aMotFwd (uint16_t duty){
	
	TA1CCR2 = 0; //set CCR2/Ain2 on @ 0
	TA1CCR1 = duty;//set CCR1/Ain1 on @ duty
	TA1CTL |= MC0;
	TA1CTL &= ~MC1;
}


void p_aMotBkwd (uint16_t duty){

	TA1CCR2 = duty;//set Ain2 on at duty
	TA1CCR1 = 0; //set CCR1/Ain1 to 0
	TA1CTL |= MC0;
	TA1CTL &= ~MC1;

}

void p_aMotCoast (void){

	TA1CCR1 = 0;
	TA1CCR2 = 0;
	TA1CTL |= MC0;
	TA1CTL &= ~MC1;

}

void motor_init(void){

	P1DIR |= (Bin1|Bin2|Ain|stby); //set would-be pwm pins to outputs
	P3DIR |= Ain;

	P1SEL0 &= ~(Bin1|Bin2|Ain|stby); //set would-be pwm pins to outputs
	P3SEL0 &= ~Ain;

	P1SEL1 &= ~(Bin1|Bin2|Ain|stby); //set would-be pwm pins to outputs
	P3SEL1 &= ~Ain;

	P1OUT |= (Bin1|Bin2|Ain|stby); //set all high(stop)
	P3OUT |= Ain;

}

void bMotFwd(void){

	P1OUT |= Bin1;
	P1OUT &= ~Bin2;

}

void bMotBkwd(void){

	P1OUT &= ~Bin1;
	P1OUT |= Bin2;

}

void bMotStop(void){

	P1OUT |= Bin1;
	P1OUT |= Bin2;

}

void bMotCoast(void){

	P1OUT &= ~(Bin1|Bin2);

}

void aMotFwd(void){

	P3OUT &= ~Ain;
	P1OUT |= Ain;

}

void aMotBkwd(void){

	P3OUT |= Ain;
	P1OUT &= ~Ain;

}

void aMotStop(void){

	P3OUT |= Ain;
	P1OUT |= Ain;

}

void aMotCoast(void){

	P3OUT &= ~Ain;
	P1OUT &= ~Ain;

}


