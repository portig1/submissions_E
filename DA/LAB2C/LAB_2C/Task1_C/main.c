/*
 * Task1_Part1_C.c
 *
 * Created: 3/13/2019 9:48:31 AM
 * Author : gausp
 */ 

#include<avr/io.h>
int main(void)
{
	
	TCCR0A = 0; // Normal Operation
	DDRB |= (1 << DDB2); //PB2 as output
	TCCR0B |= (1 << CS02) | (1 << CS00);
	// set prescaler to 1024 and start the timer
	int onCount, offCount;
	
	//For a period of 0.725s with a duty cycle of 60%, LED is on for 0.435s and off for 0.29s
	while (1)
	{
		onCount = 0;
		offCount = 0;
		TCNT0=0x00; // start the timer
		
		//Using formula TCNT = (clock/prescaler*desired_time_in_seconds) - 1, calculated that a delay of 0.435s and a prescaler of 1024 requires 6795.875 for TCNT. TCNT0 can only count to 255 so 26 iterations are needed along with an additional count to 140 to achieve approximately 0.435ms
		PORTB = (0 << 2); //Turn on LED		
		while(onCount < 26)
		{
			while ((TIFR0 & 0x01) == 0);
			TCNT0=0x00;
			TIFR0=0x01; // reset the overflow flag
			onCount++;
		};
		while(TCNT0 < 140);
		
		TCNT0=0x00; // restart the timer
		//identical calculations for 0.29s, we end up with 17 iterations and a final count up to 179
		PORTB = (1 << 2);
		while(offCount < 17)
		{
			while ((TIFR0 & 0x01) == 0);
			TCNT0=0x00;
			TIFR0=0x01; // reset the overflow flag
			offCount++;
		};
		while(TCNT0 < 179);
		
	}
}
