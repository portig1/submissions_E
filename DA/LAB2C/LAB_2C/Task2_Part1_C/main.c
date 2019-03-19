/*
 * Task2_Part1_C.c
 *
 * Created: 3/13/2019 12:59:18 PM
 * Author : gausp
 */ 

#include<avr/io.h>
#include <avr/interrupt.h>

//global variables
int overflowCounter = 0;

int main(void)
{
	DDRB |= (1 << DDB2); //PB2 as output
	TIMSK0 |= (1 << TOIE0);
	TCNT0 = 0; // initial value
	sei(); //enable interrupts
	int cycleStatus = 1; //cycle status will be for if duty cycle should be in on portion(1) or off portion(0)
	TCCR0B |= (1 << CS02) | (1 << CS00); // set prescaler to 1024 and start the timer
	
	//For a period of 0.725s, LED is on for 0.435s and then off for 0.29s. Calculations are reused from Task1_Part1
	while (1)
	{
		if((overflowCounter >= 26) & (cycleStatus == 1)) {
			while (TCNT0 < 140); 
			PORTB = (1 << 2);
			cycleStatus = 0;//set so that the LED stays off until the the cycle goes through its "off" portion
		}
		else if((overflowCounter >= 44) & (cycleStatus == 0)) {
			while(TCNT0 < 64);
			PORTB = (0 << 2);
			overflowCounter = 0; //reset cycle counter
			cycleStatus = 1;	//set so that the LED stays on until the the cycle goes through its "on" portion
			TCNT0 = 0;
		}
	}
}

ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	TCNT0 = 0;
	overflowCounter++;
}

