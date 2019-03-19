/*
 * Task3_Part1_C.c
 *
 * Created: 3/18/2019 10:08:32 AM
 * Author : gausp
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//Global variables
int overflowCounter = 0;
int cycleStatus = 1; //cycle status will be for if duty cycle should be in on portion(1) or off portion(0)

int main(void)
{
	DDRB |= (1 << DDB2); //PB2 as output
	
	OCR0A = 255; //Load Compare Reg value
	TCCR0A |= (1 << WGM01); // Set to CTC Mode
	TIMSK0 |= (1 << OCIE0A); //Set interrupt on compare match
	TCCR0B |= (1 << CS02) | (1 << CS00); // set prescaler to 1024 and starts Timer
	sei(); // enable interrupts
	while (1)
	{ // Main loop
	}
}
ISR (TIMER0_COMPA_vect)
{
	//For a period of 0.725s, LED is on for 0.435s and then off for 0.29s. Calculations are reused from Task1_Part1
	overflowCounter++;
	if((overflowCounter >= 26) & (cycleStatus == 1)) 
	{
		OCR0A = 140;
		if(overflowCounter >= 27)
		{
			OCR0A = 255;
			cycleStatus = 0; //set so that the LED stays off until the the cycle goes through its "off" portion
			PORTB = (1 << 2);
		}
	}
	else if((overflowCounter >= 44) & (cycleStatus == 0))
	{
		OCR0A = 64;
		if(overflowCounter >= 45)
		{
			OCR0A = 255;
			cycleStatus = 1; //set so that the LED stays on until the the cycle goes through its "on" portion
			PORTB = (0 << 2);
			overflowCounter = 0; //reset counter to cycle the waveform
		}
	}
}

