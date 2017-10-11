#include <msp430.h> 


/**
 * main.c
 * up timer
 */
int setF(int in)//doesn't work
{
    return (in*8591); //converts desired frequency to needed CCR0 value
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode


    P1DIR|=0x01; //sets p1 bit 0 as output
    P1SEL0|=0x00; //sets p1 bit 0 as I/O
    P1SEL1|=0x00; //sets p1 bit 0 as I/O
    P1OUT=0x00; //clears p1out before use

    P9DIR|=BIT7; //sets p1 bit 0 as output
    P9SEL0|=0x00; //sets p1 bit 0 as I/O
    P9SEL1|=0x00; //sets p1 bit 0 as I/O
    P9OUT=0x00; //clears p1out before use

    TA0CCTL0 = CCIE;//enables inerterupts

    TA0CCR0=setF(5);//sets count up value

    TA0CTL= (MC_1 + TASSEL_2 +ID_2);//set timer to up and SMCLK and internal divider to 2

    TA1CCTL0 = CCIE;//enables inerterupts

    TA1CCR0=setF(1);//sets count up value

    TA1CTL= (MC_1 + TASSEL_2 +ID_2);//set timer to up and SMCLK and internal divider to 2

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT^=BIT0;//flips led
    TA0CTL&=~BIT0;//clears flags
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    P9OUT^=BIT7;//flips led
    TA1CTL&=~BIT0;//clears flags
}
