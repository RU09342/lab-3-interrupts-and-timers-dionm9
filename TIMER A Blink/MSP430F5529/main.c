#include <msp430.h> 


/**
 * main.c
 * updown timer
 * interupt on 0
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= 0x01; // sets p1 bit 0 for output
    P1SEL=0x00;// sets p1 bit 0 to I/O
    P1OUT=0x00; //Turns led off\

    P4DIR |= BIT7; // sets p1 bit 0 for output
    P4SEL=0x00;// sets p1 bit 0 to I/O
    P4OUT=0x00; //Turns led off



    TA0CTL= ( MC__UPDOWN  + TASSEL__ACLK + ID__1 + TAIE);//updown timer, Aclk, div 1

    TA0CCR0=0x0FFF; // sets maximum value

    TA1CTL= ( MC__UPDOWN  + TASSEL__ACLK + ID__1 + TAIE);//updown timer, Aclk, div 1

    TA1CCR0=0x4FFF; // sets maximum value

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT ^= BIT0; //flips LED
    TA0CTL&=~BIT0;
}
// Timer A1 interrupt service routine
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    P4OUT ^= BIT7; //flips LED
    TA1CTL&=~BIT0;
}
