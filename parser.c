#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include "parser.h"


//extern enum for fsm
enum{AA1,AA2} state, prevState;
bool isNewState;


//returns 0 if state == raw eeg, returns error codes per state TBD
uint8_t parseChar(uint8_t c){

    isNewState = (state == prevState);
    
    prevState = state;
    
    switch(state){
        
        /*
        
        Code        Lenth(B)       Value
        0x02        N/A         Poor Quality (0-200)
        0x04        N/A         eSense Attention (0-100)
        0x05        N/A         eSense Meditation (0-100) On
        0x80        2           12-bit Raw EEG
        0x83        24          EEGPowers (integer)

        
        */
        
    }

    
}

