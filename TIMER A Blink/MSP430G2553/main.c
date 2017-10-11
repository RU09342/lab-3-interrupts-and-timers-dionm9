#include <msp430.h> 


/**
 * main.c
 * updown timer
 * interupt on 0
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT6 + BIT0; //sets p1 bit 0 to 1 for output
    P1SEL2|=0x00; //sets p1 bit 0 to 0 for I/O
    P1SEL|=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT=0x00; //clears p1 before use



    TA0CTL= ( MC_3  + TASSEL_2 + ID_1 + TAIE);//updown timer, Aclk, div 1

    TA0CCR0=0xFFFF; // sets maximum value

    TA1CTL= ( MC_3  + TASSEL_2 + ID_1 + TAIE);//updown timer, Aclk, div 1

    TA1CCR0=0xEFFF; // sets maximum value

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT ^= BIT0; //flips LED
    TA0CTL&=~BIT0;
}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    P1OUT ^= BIT6; //flips LED
    TA1CTL&=~BIT0;
}
