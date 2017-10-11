#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
//initialize LED 1
    P1DIR|=0x01; //sets p1 bit 0 as output
    P1SEL0|=0x00; //sets p1 bit 0 as I/O
    P1SEL1|=0x00; //sets p1 bit 0 as I/O
    P1OUT=0x00; //clears p1out before use
//configure button 1
    P1DIR&=~BIT1; //sets p1 bit 1 as input
    P1SEL0&=~BIT1; //sets p1 bit 1 for I/O
    P1SEL1&=~BIT1;//sets p1 bit 1 for I/O
    P1REN=BIT1;//enables p1 bit 1 resistor
    P1OUT|=BIT1; //sets p1 bit 1 resistor as pull up
    P1IE=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG =0;//clears p2 flag register

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
//port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;//flips led
    P1IFG&=~BIT1;//clears flags
}
