
#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hcsr04.h"
#include "clock.h"

#define echo1 BIT4
#define trig1 BIT5

void p2init(void){
    P2DIR |= trig1;//trig1
    P2DIR &= ~echo1;//echo1
}

uint16_t unit1_cmUltraTimer(void){
        P2OUT |= trig1; //trigger press
        delay1us(20);
        P2OUT &= ~trig1; //trigger release
        
        usTimerStart();//start timer to see if we get signal
        while(!(P2IN & echo1)){ //waiting for echo to go high
            if(usTimerCheck() > 11660){ //time out at 4m
                return 400;
            }
        }
        usTimerStart();
        while(P2IN & echo1);
        return (usTimerCheck()*0.0172);
}


