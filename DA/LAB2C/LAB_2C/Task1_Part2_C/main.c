/*
 * Task1_Part2_C.c
 *
 * Created: 3/13/2019 11:24:18 AM
 * Author : gausp
 */ 
#include<avr/io.h>
int main (void)
{
	/* set PORTB.2 for output*/
	DDRB |= (1 << 2);	
	PORTB |= (1 << 2);
	
	/* set PORTC.2 for input*/
	DDRC &= (0 << 2);
	PORTC |= (1 << 2);	//enable pull-up
	/* A switch is connected to PORTC.2 and when pressed PINC.2 is set low. */
	
	TCCR0A = 0; // Normal Operation
	TCCR0B |= (1 << CS02) | (1 << CS00); // set prescaler to 1024 and start the timer
	
	int overflowCount = 0;
	
	//From the same calculations of Task1_Part1, for a 1.25s delay 76 overflows need to occur and a count of 75 cycles.
	while (1) {
		if(!(PINC & (1 << PINC2)))
		{
			PORTB &= ~(1 << 2);
			TCNT0=0x00;	// reset counter
			TIFR0=0x01; // reset the overflow flag 
			while(overflowCount < 76)
			{
				while ((TIFR0 & 0x01) == 0);
				TCNT0=0x00;
				TIFR0=0x01; // reset the overflow flag
				overflowCount++;
			};
			while(TCNT0 < 75);
			overflowCount = 0;
		}
		else
			PORTB |= (1 << 2);
	}
	return 0;
}

