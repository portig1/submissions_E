/*
 * Task2_Part2.c
 *
 * Created: 3/13/2019 2:40:43 PM
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
	
	TIMSK0 |= (1 << TOIE0);
	TCNT0 = 0; // initial value
	sei(); //enable interrupts
	
	TCCR0A = 0; // Normal Operation
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

ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	TCNT0 = 0;
	if(switchStatus == 1) 
	{
		if(overflowCounter >= 76)
		{
			for(int i = 0; i < 75; i++);
			PORTB |= (1 << 2);
			switchStatus = 0;
			overflowCounter = 0;
		}
		overflowCounter++;
	}
}
