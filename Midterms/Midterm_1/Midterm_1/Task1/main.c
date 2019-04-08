/*
 * Task1.c
 *
 * Created: 4/7/2019 6:32:24 AM
 * Author : gausp
 */ 

/*
	Currently I've been able to send 12 temperature values to the ThingSpeak channel through the board setup. When I used a USB adapter for the ESP-01 module to 
	and ran the command sequence to send test values I was only able to send 4 values and I was repeatedly sending commands till it happened to work. Screenshots of
	ESPlorer tests are included in the documentation document
*/
#define F_CPU 16000000UL
#define BAUD_RATE 115200 //BAUD RATE for ESP01 Module
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1) //Gets ignored as the value for UBRR0 at 115200 BAUD RATE is floating point and the error rate was too high

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

void usart_init ();
void USART_send(unsigned char data);
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
	char thingSpeakUpdate[100];
	int tempC = ADCL;
	tempC = tempC | (ADCH<<8);
	tempC = (tempC/1024.0) * 5000/10;	//finish properly formatting tempC value

	char setMUX[] = "AT+CIPMUX=0\r\n"; //Repeating the steps ESPlorer used
	USART_putstring(setMUX);
	_delay_ms(1000);

	char thingSpeakStart[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
	USART_putstring(thingSpeakStart);
	_delay_ms(1000); //add in delay to allow for proper interactions

	char thingSpeakSend[] = "AT+CIPSEND=80\r\n"; //saying we'll send more data than we actually will
	USART_putstring(thingSpeakSend);
	_delay_ms(1000);
	
	snprintf(thingSpeakUpdate, sizeof(thingSpeakUpdate), "GET https://api.thingspeak.com/update?api_key=RSRMMYC8WC22KDHC&field1=%d\r\n", tempC);
	USART_putstring(thingSpeakUpdate);
	_delay_ms(1000);

	char thingSpeakClose[] = "AT+CIPCLOSE\r\n";
	USART_putstring(thingSpeakClose);
	_delay_ms(1000);
}

void usart_init (void)
{
	//UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	//UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UBRR0 = 8;	//Manually setting as the formula give a float value that has too much error.
	//UCSR0A = (1 << U2X0);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
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

