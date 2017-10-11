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
//initialize LEDs
    P1DIR=0x01; //sets P1.0 Direction to 1 for output
    P1SEL&=~BIT0; //sets P1.0 select to 0 for I/O
    P4DIR|=BIT7; //sets P4.7 direction to 1 for output
    P4SEL&=~BIT7; //sets P4.7 select to 0 for I/O
//intialize button 1
    P2REN|=BIT1; //enables P2.1 resistor for use
    P2DIR&=~BIT1; //sets P2.1 for input
    P2SEL&=~BIT1; //sets P2.1 for I/O
    P2OUT|=BIT1; //sets P2.1 resistor as a pullup resistor
    P2IE=BIT1;//enable interupt detection
    P2IES&=~BIT1;//flags interupt on negative
    P2IFG&=~BIT1;//clears p5 flag register

//initialize button 2
    P1REN|=BIT1; //sets P1.1 REN to 1 to enable resistor
    P1DIR&=~BIT1; //sets P1.1 direction for input
    P1SEL&=~BIT1; //sets P1.1 select for I/O
    P1OUT|=BIT1; //sets P1.1 resistor as a pullup resistor
    P1IE|=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative edge
    P1IFG&=~BIT1;//clears p5 flag register

//setup timers A0 and A1
    TA0CTL= (MC_3 + TASSEL_2 +ID_3);//set timerA0 to up/down, SMCLK as clock input, and clock divider to 8
    TA1CTL= (MC_3 + TASSEL_2 +ID_3);//set timerA1 to up/down, SMCLK as clock input, and clock divider to 8
//enable timer interupts
    TA0CCTL0 = CCIE;//enable interupts on TA0CCR0
    TA1CCTL0 = CCIE;//enable interupts on TA1CCR0
//set timer CCRs
    TA0CCR0=3125;//sets timer0 to 10 Hz
    TA1CCR0=31250;//sets timer1 to 1 Hz

    _BIS_SR(LPM0_bits + GIE);//enters low power mode 0 with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    P1OUT^=BIT0;//flips led 1
}
//Timer A1 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
    P4OUT^=BIT7;//flips led 2
}
//Button interrupt service routines
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN)&BIT1)==BIT1){//check if button 1 is pressed
            TA0CCR0=setF(10);//set frequency to 10
            P1IFG&=~BIT1;//clears flags
        }

}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    int newF;//int for new frequency
          newF=1;//set initial frequency to 1

          if(((~P2IN&BIT1)==BIT1)){//checks if button 2 is pressed
                  TA1R=0x0000;//sets 1 hz timer value to 0
                  while((((~P2IN)&BIT1)==BIT1)){//checks if button is pressed
                      if((TA1CTL&BIT0)==BIT0){//checks if 1 second has passed
                              TA0CCR0=setF(newF);//sets new frequency
                              newF=newF+1;//increments new frequency
                              TA1CTL&=~BIT0;//clears timer flags
                      }
                      else{
                          newF=newF;//holds newF value while a full second has not passed
                      }
                  }

              P2IFG&=~BIT1;//clears button 2 flag
              }

}
