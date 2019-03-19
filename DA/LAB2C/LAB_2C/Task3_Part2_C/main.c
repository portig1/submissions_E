/*
 * Task3_Part2_C.c
 *
 * Created: 3/18/2019 10:39:39 AM
 * Author : gausp
 */ 
#include<avr/io.h>
#include<avr/interrupt.h>

int switchStatus = 0; //1 = pressed, 0 = not pressed
int overflowCounter = 0; //counts overflows
int main (void)
{
	/* set PORTB.2 for output*/
	DDRB |= (1 << 2);
	PORTB |= (1 << 2);
	
	/* set PORTC.2 for input*/
	DDRC &= (0 << 2);
	PORTC |= (1 << 2);	//enable pull-up
	/* A switch is connected to PORTC.2 and when pressed PINC.2 is set low. */
	
	OCR0A = 255; //Load Compare Reg value
	
	TIMSK0 |= (1 << OCIE0A);
	sei(); //enable interrupts
	
	TCCR0A |= (1 << WGM01); // Set to CTC Mode
	TCCR0B |= (1 << CS02) | (1 << CS00); // set prescaler to 1024 and start the timer

	
	//From the same calculations of Task1_Part2, for a 1.25s delay 76 overflows need to occur and a count of 75 cycles.
	while (1) {
		if(!(PINC & (1 << PINC2)))
		{
			PORTB &= ~(1 << 2);
			switchStatus = 1;
		}
	}
	return 0;
}

ISR (TIMER0_COMPA_vect) // timer0 compare interrupt
{
	if(switchStatus == 1)
	{
		if(overflowCounter >= 76)
		{
			OCR0A = 75;
			if(overflowCounter >= 77)
			{
				PORTB |= (1 << 2);
				switchStatus = 0;
				overflowCounter = 0;
				OCR0A = 255;
			}
		}
		overflowCounter++;
	}
}
