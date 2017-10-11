#include <msp430.h> 


/**
 * main.c
 * Continuous timer interupt
 * needs nothing
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    P1DIR |= BIT0+BIT1; // sets p1 bit 0 and 1 for output
    P1SEL1=0x00;// sets p1 bit 0 to I/O
    P1SEL0=0x00;//sets p1 bit 0 to I/O
    P1OUT=0x00; //Turns led off

    TA0CTL=(TAIE + MC_2 + TASSEL__SMCLK);//set timer to continuous

    TA1CTL=(TAIE + MC_2 + TASSEL__SMCLK + ID__8);//set timer to continuous

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    switch(TA0IV)
      case 0x0E: P1OUT ^= BIT0; //flips LED
      TA0CTL&= ~BIT0;
}
// Timer A0 interrupt service routine
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    switch(TA1IV)
      case 0x0E: P1OUT ^= BIT1; //flips LED
      TA0CTL&= ~BIT0;
}
