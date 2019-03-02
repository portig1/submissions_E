/*
 * Task1_C.c
 *
 * Created: 2/28/2019 7:43:52 PM
 * Author : gausp
 */ 

#include <avr/io.h>
/* 	- LED connected to PORTB.2 */

int main (void)
{
	/* set PORTB.2 for output*/
	DDRB |= (1 << 2);	
	PORTB |= (1 << 2);
	long i;
	/* Toggle PORTB.2 on and off. On for 435ms and off for 290ms to achieve a period of 725ms with a duty cycle of 60%
		The LED connected to PORTB is active low so by setting PORTB.2 to 0, it is actually being turned on */
	/* For a counter set to 16,000,000, it takes 96000004 clock cycles to complete a countdown and it takes 6s to complete 
		using this ratio, I calculated to get a time of 0.435s the counter needs to be set around 1,160,000 and for 0.29s a counter of 773,333 */
	while (1)	{
		i = 1160000;	
		PORTB = _BV(PINB2);
		while (i > 0) {
			i--;
		};
		i = 773333;
		PORTB = ~(_BV(PINB2));
		while (i > 0) {
			i--;
		};
	}
	return 0;
}

