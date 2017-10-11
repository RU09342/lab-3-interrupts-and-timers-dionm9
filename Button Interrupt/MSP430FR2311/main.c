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
    P1DIR|=0x01; //set p1 bit 0 for output
    P1SEL0|=0x00; //set p1 bit 0 for I/O
//configure button 1
    P1OUT|=BIT1; //selected Vcc for pull up resistor on bit 1
    P1REN|=BIT1; //enabled pull up resistor on bit 1
    P1DIR&=~BIT1; //set p1 for input on bit 1
    P1SEL1|=0x00; //sets p1 for I/O
    P1IE=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG =0;//clears p1 flag register

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts

}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;//flips led
    P1IFG&=~BIT1;//clears flags
}
