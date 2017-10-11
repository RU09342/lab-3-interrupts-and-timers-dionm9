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
//initilize LEDs
    P1DIR |= BIT0+BIT1; // sets P1.0 and P1.1 direction to 1 for output
    P1SEL1&=BIT0;// sets P1.0 select1 to 0 for I/O
    P1SEL0&=BIT0;//sets P1.0 select 2 to 0 for I/O
    P1OUT&=BIT0; //Sets P1.0 output to 0
//intialize button 1
    P5REN|=BIT6; //enables resistor for P5.6
    P5DIR&=~BIT6; // sets P5.6 for input
    P5SEL1&=~BIT6; //sets P5.6 for I/O
    P5SEL0&=~BIT6; //sets P5.6 for I/O
    P5OUT|=BIT6; // sets P5.6 resistor for pull up
    P5IES&=~BIT6;//flags interupt on negative
    P5IE|=BIT6;//enable interupt detection
    P5IFG&=BIT5;//clears P5.6 flag register
//initialize button 2
    P5REN|=BIT5; //sets P5.5 REN to 1 to enable resistor
    P5DIR&=~BIT5; // sets P5.5 direction to 0 for input
    P5SEL1&=~BIT5; //sets P5.5 select1 to 0 for I/O
    P5SEL0&=~BIT5; //sets P5.5 select0 to 0 for I/O
    P5OUT|=BIT5; // sets P5.5 to 1 to set resistor to pull up
    P5IES&=~BIT5;//flags interupt on negative
    P5IE|=BIT5;//enable interupt detection
    P5IFG&=BIT5;//clears P5.5 flag register
//setup timers A0 and A1
    TA0CTL= (MC_3 + TASSEL_2 +ID_3);//set timer0 to up/down, selects SMCLK, and clock divider to 8
    TA1CTL= (MC_3 + TASSEL_2 +ID_3);//set timer1 to up/down, selects SMCLK, and clock divider to 8
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
    P1OUT^=BIT1;//flips led
}
//Button interrupt service routines
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    int newF;//int for new frequency
      newF=1;//set initial frequency to 1

    if(((~P5IN)&BIT5)==BIT5){//check if button 1 is pressed
            TA0CCR0=setF(10);//set frequency to 10
            P5IFG&=~BIT5;//clears button 1 flags
        }
    if(((~P5IN&BIT6)==BIT6)){//checks if button 2 is pressed
        TA1R=0x0000;//sets 1 hz clock to 0
        while((((~P5IN)&BIT6)==BIT6)){//checks if button is pressed
            if((TA1CTL&BIT0)==BIT0){//checks if 1 second has passed
                    TA0CCR0=setF(newF);//sets new frequency
                    newF=newF+1;//increments new frequency
                    TA1CTL&=~BIT0;//clears timer flags
            }
            else{
                newF=newF;//holds newF value while a full second has not passed
            }
        }

    P5IFG&=~BIT6;//clears button 2 flag
    }
}
