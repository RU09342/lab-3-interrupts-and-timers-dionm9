
#include <msp430.h> 
/**
 * main.c
 * up timer interupt on CCR1 and CCR2
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    P1DIR=0x01; //sets p1 bit 0 as an output
    P1SEL0=0x00; //sets p1 bit 0 as I/O
    P1OUT=0x00; //Turns led off
    P2DIR=0x01; //sets p1 bit 0 as an output
    P2SEL0=0x00; //sets p1 bit 0 as I/O
    P2OUT=0x00; //Turns led off

    TB0CTL= ( MC__UP  + TBSSEL__SMCLK + ID__1);//updown timer, Aclk, div 1

    TB0CCTL1 = (CCIE);

    TB0CCTL2 = (CCIE);

    TB0CCR0=0xFFFF; // sets maximum timer value

    TB0CCR1=0xDFFF; // sets CCR1 interupt value

    TB0CCR2=0x0FFF; // sets CCR2 interupt value

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    if ((TB0CCTL1&BIT0)==BIT0){
        P1OUT ^= BIT0; //flips LED 1
        TB0CCTL1&=~BIT0; //clears flags
    }
    if ((TB0CCTL2&BIT0)==BIT0){
        P2OUT ^= BIT0; //flips LED 2
        TB0CCTL2&=~BIT0; //clears flags
    }
}
