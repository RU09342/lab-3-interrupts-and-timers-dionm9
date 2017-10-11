#include <msp430.h> 


/**
 * main.c
 */
int setF(int in)
{
    return (31250/(in)); //converts desired frequency to needed CCR0 value
}
int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode
//configure LED 1
    P1DIR |= BIT0; //sets P1.0 direction to 1 for output
    P1SEL1&=BIT0;// sets P1.0 select1 to 0 for I/O
    P1SEL0&=BIT0;//sets P1.0 select 2 to 0 for I/O
    P1OUT&=BIT0; //Sets P1.0 output to 0 to clear the pin
//configure LED 2
    P9DIR|=BIT7;//sets P9.7 direction to 1 for output
    P9SEL0&=~BIT7;//sets P9.7 select2 to 0 for I/O
    P9SEL1&=~BIT7;//sets P9.7 select1 to 0for I/O
    P9OUT&=~BIT7; //clears p9out before use
//configure buttons
    P1DIR&=~(BIT1+BIT2); //sets P1.1 and P1.2 direction to 0 to set pins as inputs
    P1SEL0&=~(BIT1+BIT2); //sets P1.1 and P1.2 to 0 for I/O
    P1SEL1&=~(BIT1+BIT2);//sets P1.1 and P1.2 to 0 for I/O
    P1REN=BIT1+BIT2;//enables P1.1 and P1.2 resistors
    P1OUT|=BIT1+BIT2; //sets P1.1 and P1.2 resistor as pull up
    P1IE=BIT1+BIT2;//enable interupt detection for P1.1 and P1.2
    P1IES&=~(BIT1+BIT2);//flags interupts on P1.1 and P1.2 negative edge
    P1IFG&=~(BIT1+2);//clears p2 flag register
//setup timers A0 and A1
    TA0CTL= (MC_3 + TASSEL_2 +ID_3);//set timer0 to up/down mode, selects SMCLK as clock input, and sets clock divider to 8
    TA1CTL= (MC_3 + TASSEL_2 +ID_3);//set timer1 to up/down mode, selects SMCLK as clock input, and sets clock divider to 8
//enable timer interupts
    TA0CCTL0 = CCIE;//enable interupts on TA0CCR0
    TA1CCTL0 = CCIE;//enable interupts on TA1CCR0
//set timer CCRs
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
    P9OUT^=BIT7;//flips led
}
//Button interrupt service routines
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    int newF;//int for new frequency
      newF=1;//set initial frequency to 1

    if(((~P1IN)&BIT2)==BIT2){//check if button 1 is pressed
            TA0CCR0=setF(10);//set frequency to 10
            P1IFG&=~BIT2;//clears flags
        }
    if(((~P1IN&BIT1)==BIT1)){//checks if button 2 is pressed
        TA1R=0x0000;//sets 1 hz clock to 0
        while((((~P1IN)&BIT1)==BIT1)){//checks if button is pressed
            if((TA1CTL&BIT0)==BIT0){//checks if 1 second has passed
                    TA0CCR0=setF(newF);//sets new frequency
                    newF=newF+1;//increments new frequency
                    TA1CTL&=~BIT0;//clears timerA1 flags
            }
            else{
                newF=newF;//holds newF value while a full second has not passed
            }
        }

    P1IFG&=~BIT2;//clears button 2 flag
    }
}
