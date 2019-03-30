/*
 * LAB_3B_Task1.c
 *
 * Created: 3/29/2019 6:08:33 PM
 * Author : gausp
 */ 

/*
 * The program reads the LM35 temperature of the MCU using ADC and sends it to the PC.
 * If you put your finger on the MCU, the number increases.
 *
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>		
#include <avr/interrupt.h>
#include <stdio.h>

void usart_init ();
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);


int main (void)
{
	usart_init ();
	
	sei();
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 62499; //Using TCNT = clk*delay/prescaler - 1 to find OCR1A given clk = 16MHz, OCR1A was calculated to 62,499
	TCCR1A = 0; // COM1A/B Normal Operation, OC1A/B Disconnected
	TCCR1B = (1 << WGM12) | (1 << CS12); //WGM CTC Mode, Prescaler = 256
	
   /** Setup and enable ADC **/
   ADMUX = (0<<REFS1)|    // Reference Selection Bits
   (1<<REFS0)|    // AVcc - external cap at AREF
   (0<<ADLAR)|    // ADC Left Adjust Result
   (1<<MUX2)|     // Analog Channel Selection Bits
   (0<<MUX1)|     // ADC4 (PC4 PIN27)
   (0<<MUX0);
   ADCSRA = (1<<ADEN)|    // ADC Enable
   (0<<ADSC)|     // ADC Start Conversion
   (0<<ADATE)|    // ADC Auto Trigger Enable
   (0<<ADIF)|     // ADC Interrupt Flag
   (0<<ADIE)|     // ADC Interrupt Enable
   (1<<ADPS2)|    // ADC Prescaler Select Bits
   (0<<ADPS1)|
   (1<<ADPS0);
	
	while (1)
	{

	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	ADCSRA|=(1<<ADSC);	//start conversion
	while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
	
	ADCSRA |= (1<<ADIF);
	char output[20];
	int tempC = ADCL;
	tempC = tempC | (ADCH<<8);
	tempC = (tempC/1024.0) * 5000/10;
	
	snprintf(output, sizeof(output), "%d\r\n", tempC);
	USART_putstring(output);
}

void usart_init (void)
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
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