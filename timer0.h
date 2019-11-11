
#ifndef TIMER0_H
#define TIMER0_H

#include <xc.h>

INTCONbits.PEIE     =   1;
INTCONbits.TMR0IE   =   1;
INTCONbits.TMR0IF   =   0;
T0CONbits.T0CS      =   0;
T0CONbits.PSA       =   0;
T0CONbits.T0PS      =   0b111;

//Tratador de interrupção do timer
void interrupt() timer_zero_int();

#endif