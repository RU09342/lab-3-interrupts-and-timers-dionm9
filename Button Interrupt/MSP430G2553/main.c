#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
//initialize LED 1
    P1DIR |= 0x01; //sets p1 bit 0 to 1 for output
    P1SEL2|=0x00; //sets p1 bit 0 to 0 for I/O
    P1SEL|=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT|=0x00; //clears p1 before use
//configure button 1
    P1DIR&=~BIT3; //sets p1 bit 3 to 0 for button Input
    P1SEL&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1SEL2&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1REN|=BIT3; //enables p1 bit 3 resistor for use
    P1OUT|=BIT3; //sets p1 bit 3 resistor to pull up resistor
    P1IE=BIT3;//enable interupt detection
    P1IES&=~BIT3;//flags interupt on negative
    P1IFG =0;//clears p2 flag register

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;//flips led
    P1IFG&=~BIT3;//clears flags
}
