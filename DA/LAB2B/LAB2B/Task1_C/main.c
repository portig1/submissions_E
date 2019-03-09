/*
 * LAB2B.c
 *
 * Created: 3/8/2019 4:41:20 PM
 * Author : gausp
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRB |= (1 << 2); //PB2 as output
	PORTB |= (1 << 2); //Turn off LED (reverse logic)
	PORTD |= (1 << 2); //Pull-up activated
	EICRA = 0x02; //Make INT0 falling-edge triggered
	
	EIMSK = (1 << INT0); //enable external interrupt 0
	sei(); //enable interrupts
	
	while(1) { 
	}
}

ISR (INT0_vect) { //ISR for external interrupt 0 
	
	PORTB = (0 << 2); //Turn on 
	
	/* For a counter set to 16,000,000 it takes 6s to complete and using that ratio and knowing I need a delay of 1.25s, I calculated
	that the counter needs to be set at 3,333,333. However, when running on hardware with this loop in the interrupt, the delay takes about
	twice as long to complete. So for this I halved 3,333,333 to 1,666,667 to get the 1.250s delay*/
	
	long i; //initialize i for counter
	i = 1666667;
	while (i > 0) {	
		i--;
	};
	PORTB = (1 << 2); //Turn off
}