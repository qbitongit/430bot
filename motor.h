#ifndef ROT_H
#define ROT_H
#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

void motor_init(void);

void aMotFwd(void);

void aMotBkwd(void);

void aMotStop(void);

void aMotCoast(void);

void bMotFwd(void);

void bMotBkwd(void);

void bMotStop(void);

void bMotCoast(void);

void motorpwm_init(void);

void p_aMotFwd (uint16_t duty);

void p_aMotBkwd (uint16_t duty);

void p_aMotCoast (void);

void p_bMotFwd (uint16_t duty);

void p_bMotBkwd (uint16_t duty);

void p_bMotCoast (void);


#endif
