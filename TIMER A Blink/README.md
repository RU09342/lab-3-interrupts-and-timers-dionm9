# TIMER A Blink
Each of the TimerA blink programs were different. Each one used their own combination of different clocks, dividers, and timer modes. The program blinks LED 1 at a rate sets by TA0CCR0, and blink LED 2 at a rate of TA1CCR0. All three timer modees, both clock inputs ,aclk and smclk were used, along with different values for the internal dividers. 

### Extra Work
I made a the program run on the FR5994 in LPM4 on the Super capacitor. The program ran for 3 minutes 20 seconds after only a 5 miniute charge. I attempted to set the unused GPIO pints to high impedence mode (not sure if it worked). The program configures the device for operation and configures LED 1. The 
