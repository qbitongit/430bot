#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "clock.h"



void TA3ClockInit(void){
    TA3CTL = (TASSEL_2|MC_2|TACLR);
}

void delay1us(uint16_t num){
    TA3CTL |= TACLR;//clear timer
    while(TA3R <= num);
}

void delay1ms(uint16_t num){
    uint16_t i;
    for(i = num; i>0; i--){
        delay1us(1000);
    }
}

//timer counts to 65ms then rolls

void usTimerStart(void){
    TA3CTL |= TACLR;
}


uint16_t usTimerCheck(void){
    return TA3R;
}

