#include <msp430.h> 


/**
 * main.c
 * Button Based Delay
 * using continuous mode
 */

int setF(int in)
{
    return (31250/(in)); //converts desired frequency to needed CCR0 value
}

int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
//initialize LED 1
    P1DIR |= 0x01; //sets p1 bit 0 as an output
    P1SEL2|=0x00; //sets p1 bit 0 for I/O
    P1SEL|=0x00;//sets p1 bit 0 for I/O
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
//initialize LED 2
    P1DIR |= BIT6;//sets p1 bit 6 as an output
    P1SEL2&=~BIT6;//sets p1 bit 6 for I/O
    P1SEL&=~BIT6;//sets p1 bit 6 for I/O
    P1OUT&=~BIT6;//clears p1 before use
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
//initialize button
    P1DIR&=~BIT3; //sets p1 bit 3 to 0 for button Input
    P1SEL&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1SEL2&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1REN|=BIT3; //enables p1 bit 3 resistor for use
    P1OUT|=BIT3; //sets p1 bit 3 resistor to pull up resistor
    P1IE=BIT3;//enable interupt detection
    P1IES&=~BIT3;//flags interupt on negative
//initialize timers
    TA0CTL= (MC_3 + TASSEL_2 +ID_3);//set timer0 to up/down mode, SMCLK as clock input, and internal divider to 8
    TA1CTL= (MC_3 + TASSEL_2 +ID_3);//set timer1 to up/down mode, SMCLK as clock input, and internal divider to 8
//enable timer interupts
    TA0CCTL0 = CCIE;//enable interupt on TA0CCR0
    TA1CCTL0 = CCIE;//enable interupt on TA1CCR0
//set timer frequencies
    TA0CCR0=3125;//sets timer0 to 10 Hz
    TA1CCR0=31250;//sets timer1 to 1 Hz

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT^=BIT0;//flips led
}
//Timer A1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    P1OUT^=BIT6;//flips led
}
//Button interrupt service routines
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    int newF;//int for new frequency
      newF=1;//set initial frequency to 1

    if(((~P1IN&BIT3)==BIT3)){//checks if button 2 is pressed
        TA1R=0x0000;//sets 1 hz clock to 0
        while((((~P1IN)&BIT3)==BIT3)){//checks if button is pressed
            if((TA1CTL&BIT0)==BIT0){//checks if 1 second has passed
                    TA0CCR0=setF(newF);//sets new frequency
                    newF=newF+1;//increments new frequency
                    TA1CTL&=~BIT0;//clears timer flags
            }
            else{
                newF=newF;//holds newF value while a full second has not passed
            }
        }

    P1IFG&=~BIT3;//clears button 2 flag
    }
}
