# Button Based Delay
The program works by first initializing the device for operation, initializing the LED(s), initializing the buttons. Timer0 is then set to a frequency of 10 Hz and timer1 is then set to a frequency of 1Hz. When button 1 is pressed, it triggers an interupt. The program then tracks the number of seconds (through timer 1) the button is pressed for and sets that to the LED frequency (writes to TA0CCR0).
## Extra Work
### Reset Button
A reset button was created to set the frequency back to 10 Hz. This works by setting the TA0CCR0 value to a specific value on the reset button interupt.
