/*
 * Task2_C.c
 *
 * Created: 3/2/2019 9:05:36 AM
 * Author : gausp
 */ 

#include <avr/io.h>
/* 	- LED connected to PORTB.2 */
/*	- Switch connected to PORTC.2 */

int main (void)
{
	long i; //initialize i for counter
	/* set PORTB.2 for output*/
	DDRB |= (1 << 2);	
	PORTB |= (1 << 2);
	
	/* set PORTC.2 for input*/
	DDRC &= (0 << 2);
	PORTC |= (1 << 2);	//enable pull-up
	
	/* A switch is connected to PORTC.2 and when pressed PINC.2 is set low. Condition for the if statment is then true
	and the LED will turn on for 1.25s. Using the ratio found in Task1_ I calculated the counter needs to be set to
	333,333 to have a delay of approximately 1.25s*/
	
	while (1) {
		if(!(PINC & (1 << PINC2)))
		{
			PORTB &= ~(1 << 2);
			i = 3333333;
			while (i > 0) {
				i--;
			};
		}
		else
			PORTB |= (1 << 2);
		
	}
	return 0;
}

