#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
//initialize LED 1
    P1DIR |= 0x01; // sets p1 bit 0 for output
    P1SEL1=0x00;// sets p1 bit 0 to I/O
    P1SEL0=0x00;//sets p1 bit 0 to I/O
//configure button 1
    P5REN|=BIT5; //enables resistor for P1.5
    P5DIR&=~BIT5; // sets P1.5 for input
    P5SEL1&=~BIT5; //sets P1.5 for I/O
    P5SEL0&=~BIT5; //sets P1.5 for I/O
    P5OUT|=BIT5; // sets P1.5 resistor for pull up
    P5IE=BIT5;//enable interupt detection
    P5IES&=~BIT5;//flags interupt on negative

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
//port 5 ISR
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    P1OUT^=BIT0;//flips led
    P5IFG&=~BIT5;//clears flags
}
