/*
 * TASK1.c
 *
 * Created: 4/17/2019 6:35:34 AM
 * Author : gausp
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

void stepperDelay(int delayInMilliseconds);
void usart_init ();
void USART_send(unsigned char data);
void USART_putstring(char* StringPtr);

int main (void)
{
	DDRB = 0xFF;
	usart_init ();

	
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(0<<MUX2)|     // Analog Channel Selection Bits
	(0<<MUX1)|     // ADC0 (PC0) Potentionmeter
	(0<<MUX0);
	
	ADCSRA = (1<<ADEN)|    // ADC Enable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|	   // CLK/32
	(1<<ADPS0);
	
	int delay1 = 20; //20ms delay
	int delay2 = 40; //40ms delay
	int delay3 = 60;
	int delay4 = 80;
	int delay5 = 100;
	int delay6 = 120;
	int delay7 = 140;
	int delay8 = 160;
	int delay9 = 180;
	int delay10 = 200;
	
	while (1)
	{
		
		ADCSRA|=(1<<ADSC);	//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);
		
		int tempC = ADCL;
		tempC = tempC | (ADCH<<8);
		
		char output[20];
		snprintf(output, sizeof(output), "%d\r\n", tempC); //prints out potentiometer value to serial terminal
		USART_putstring(output);

		//When the ADC value from the potentiometer is below 10, a duty cycle of 0% will be output. Then above 10 and below 20, a duty cycle of 10%. This goes on until the value is above
		//90 for which only a 95% duty cycle will be produced
		if((0 <= tempC) & (tempC < 10))
		{
			PORTB=0x09;
			stepperDelay(delay1);
			PORTB=0x08;
			stepperDelay(delay1);
			PORTB=0x0C;
			stepperDelay(delay1);
			PORTB=0x04;
			stepperDelay(delay1);
			PORTB=0x06;
			stepperDelay(delay1);
			PORTB=0x02;
			stepperDelay(delay1);
			PORTB=0x03;
			stepperDelay(delay1);
			PORTB=0x01;
			stepperDelay(delay1);
		}
		else if((10 <= tempC) & (tempC < 20))
		{
			PORTB=0x09;
			stepperDelay(delay2);
			PORTB=0x08;
			stepperDelay(delay2);
			PORTB=0x0C;
			stepperDelay(delay2);
			PORTB=0x04;
			stepperDelay(delay2);
			PORTB=0x06;
			stepperDelay(delay2);
			PORTB=0x02;
			stepperDelay(delay2);
			PORTB=0x03;
			stepperDelay(delay2);
			PORTB=0x01;
			stepperDelay(delay2);
		}
		else if((20 <= tempC) & (tempC < 30))
		{
			PORTB=0x09;
			stepperDelay(delay3);
			PORTB=0x08;
			stepperDelay(delay3);
			PORTB=0x0C;
			stepperDelay(delay3);
			PORTB=0x04;
			stepperDelay(delay3);
			PORTB=0x06;
			stepperDelay(delay3);
			PORTB=0x02;
			stepperDelay(delay3);
			PORTB=0x03;
			stepperDelay(delay3);
			PORTB=0x01;
			stepperDelay(delay3);
		}
		else if((30 <= tempC) & (tempC < 40))
		{
			PORTB=0x09;
			stepperDelay(delay4);
			PORTB=0x08;
			stepperDelay(delay4);
			PORTB=0x0C;
			stepperDelay(delay4);
			PORTB=0x04;
			stepperDelay(delay4);
			PORTB=0x06;
			stepperDelay(delay4);
			PORTB=0x02;
			stepperDelay(delay4);
			PORTB=0x03;
			stepperDelay(delay4);
			PORTB=0x01;
			stepperDelay(delay4);
		}
		else if((40 <= tempC) & (tempC < 50))
		{
			PORTB=0x09;
			stepperDelay(delay5);
			PORTB=0x08;
			stepperDelay(delay5);
			PORTB=0x0C;
			stepperDelay(delay5);
			PORTB=0x04;
			stepperDelay(delay5);
			PORTB=0x06;
			stepperDelay(delay5);
			PORTB=0x02;
			stepperDelay(delay5);
			PORTB=0x03;
			stepperDelay(delay5);
			PORTB=0x01;
			stepperDelay(delay5);
		}
		else if((50 <= tempC) & (tempC < 60))
		{
			PORTB=0x09;
			stepperDelay(delay6);
			PORTB=0x08;
			stepperDelay(delay6);
			PORTB=0x0C;
			stepperDelay(delay6);
			PORTB=0x04;
			stepperDelay(delay6);
			PORTB=0x06;
			stepperDelay(delay6);
			PORTB=0x02;
			stepperDelay(delay6);
			PORTB=0x03;
			stepperDelay(delay6);
			PORTB=0x01;
			stepperDelay(delay6);
		}
		else if((60 <= tempC) & (tempC < 70))
		{
			PORTB=0x09;
			stepperDelay(delay7);
			PORTB=0x08;
			stepperDelay(delay7);
			PORTB=0x0C;
			stepperDelay(delay7);
			PORTB=0x04;
			stepperDelay(delay7);
			PORTB=0x06;
			stepperDelay(delay7);
			PORTB=0x02;
			stepperDelay(delay7);
			PORTB=0x03;
			stepperDelay(delay7);
			PORTB=0x01;
			stepperDelay(delay7);
		}
		else if((70 <= tempC) & (tempC < 80))
		{
			PORTB=0x09;
			stepperDelay(delay8);
			PORTB=0x08;
			stepperDelay(delay8);
			PORTB=0x0C;
			stepperDelay(delay8);
			PORTB=0x04;
			stepperDelay(delay8);
			PORTB=0x06;
			stepperDelay(delay8);
			PORTB=0x02;
			stepperDelay(delay8);
			PORTB=0x03;
			stepperDelay(delay8);
			PORTB=0x01;
			stepperDelay(delay8);
		}
		else if((80 <= tempC) & (tempC < 90))
		{
			PORTB=0x09;
			stepperDelay(delay9);
			PORTB=0x08;
			stepperDelay(delay9);
			PORTB=0x0C;
			stepperDelay(delay9);
			PORTB=0x04;
			stepperDelay(delay9);
			PORTB=0x06;
			stepperDelay(delay9);
			PORTB=0x02;
			stepperDelay(delay9);
			PORTB=0x03;
			stepperDelay(delay9);
			PORTB=0x01;
			stepperDelay(delay9);
		}
		else
		{
			PORTB=0x09;
			stepperDelay(delay10);
			PORTB=0x08;
			stepperDelay(delay10);
			PORTB=0x0C;
			stepperDelay(delay10);
			PORTB=0x04;
			stepperDelay(delay10);
			PORTB=0x06;
			stepperDelay(delay10);
			PORTB=0x02;
			stepperDelay(delay10);
			PORTB=0x03;
			stepperDelay(delay10);
			PORTB=0x01;
			stepperDelay(delay10);
		}
		
	}
	return 0;
}



void usart_init (void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}


void USART_send( unsigned char data) {

	while (!(UCSR0A & (1 << UDRE0))); //wait until UDR0 is empty
	UDR0 = data;					 //transmit ch

}

void USART_putstring(char* StringPtr) {

	while (*StringPtr != 0x00) {
		USART_send(*StringPtr);
		StringPtr++;
	}

}

void stepperDelay(int delayInMilliseconds) 
{
	TCNT1 = 0;
	OCR1A = (((F_CPU/64)/1000)*delayInMilliseconds)-1;
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); //CTC mode, CLK/64 for maximum delay of ~260ms
	while(TCNT1 < OCR1A); //Wait until TCNT is equal to OCR1A
	TCCR1B = 0; //Stop timer
}