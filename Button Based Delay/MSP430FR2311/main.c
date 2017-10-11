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
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
//initialize LED 1
    P1DIR|=BIT0; //set p1 bit 0 for output
    P1SEL0&=~BIT0; //set p1 bit 0 for I/O
//initialize LED 2
    P2DIR|=0x01; //set p1 bit 0 for output
    P2SEL0|=0x00; //set p1 bit 0 for I/O
//initialize button
    P1OUT|=BIT1; //selected Vcc for pull up resistor on bit 1
    P1REN|=BIT1; //enabled pull up resistor on bit 1
    P1DIR&=~BIT1; //set p1 for input on bit 1
    P1SEL1&=~BIT1; //sets p1 for I/O
    P1IE|=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG&=~BIT1;//clears p2 flag register
//initialize timers
    TB0CTL= (MC_3 + TBSSEL_2 +ID_3);//set timerb0 to up/down mode, SMCLK as clock input, and internal divider to 1
    TB1CTL= (MC_3 + TBSSEL_2 +ID_3);//set timerb1 to up/down mode, SMCLK as clock input, and internal divider to 1
//enable timer interupts
    TB0CCTL0 = CCIE;//enable interupt on TB0CCR0
    TB1CCTL0 = CCIE;//enable interupt on TB1CCR0
//set timer frequencies
    TB0CCR0=3125;//sets timer0 to 10 Hz
    TB1CCR0=31250;//sets timer1 to 1 Hz

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    P1OUT^=BIT0;//flips led
}
//Timer B1 interrupt service routine
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR (void)
{
    P2OUT^=BIT0;//flips led
}
//Button interrupt service routines
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    int newF;//int for new frequency
      newF=1;//set initial frequency to 1

    if(((~P1IN&BIT1)==BIT1)){//checks if button 2 is pressed
        TB1R=0x0000;//sets 1 hz clock to 0
        while((((~P1IN)&BIT1)==BIT1)){//checks if button is pressed
            if((TB1CTL&BIT0)==BIT0){//checks if 1 second has passed
                    TB0CCR0=setF(newF);//sets new frequency
                    newF=newF+1;//increments new frequency
                    TB1CTL&=~BIT0;//clears timer flags
            }
            else{
                newF=newF;//holds newF value while a full second has not passed
            }
        }

    P1IFG&=~BIT1;//clears button 2 flag
    }
}
