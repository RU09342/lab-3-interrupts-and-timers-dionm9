#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//initialize device
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
//initialize LED 1
	P1DIR=0x03; //sets p1 bit 0 to 1 for output
    P1SEL=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT=0x00; // clears p1 out before use
//initialize LED 2
    P2DIR=0x03; //sets p1 bit 0 to 1 for output
    P2SEL=0x00; //sets p1 bit 0 to 0 for I/O
    P2OUT=0x00; // clears p1 out before use
//configure button 1
    P2REN|=BIT1; //enables p2 bit 1 resistor for use
    P2DIR&=~BIT1; //sets p2 bit 1 for input
    P2SEL&=~BIT1; //sets p2 bit 1 for I/O
    P2OUT|=BIT1; //sets p2 bit 1 resistor as a pullup resistor
    P2IE=BIT1;//enable interupt detection
    P2IES&=~BIT1;//flags interupt on negative
    P2IFG =0;//clears p2 flag register

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
//Port 2 ISR
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P1OUT^=BIT0;//flips led
    P2IFG&=~BIT1;//clears flags
}
