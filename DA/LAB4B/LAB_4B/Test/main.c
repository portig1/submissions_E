/*
 * Test.c
 *
 * Created: 4/20/2019 7:14:33 AM
 * Author : gausp
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB = 0xFF;
	DDRB = 0x00;
    DDRB = 0xFF;
    while (1) 
    {
		PORTB = 0xFF;
		PORTB = 0;
    }
}

