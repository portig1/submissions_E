/*
 * TASK2.c
 *
 * Created: 4/21/2019 7:16:27 PM
 * Author : gausp
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void adc_init();
int adc_read();
void usart_init ();
void USART_send(unsigned char data);
void USART_putstring(char* StringPtr);

int main(void)
{
	usart_init();
	adc_init();
	
	//Using information from slide 15-16 from Lecture 11 presentation
	TCNT1 = 0;		// Set timer1 count zero
	ICR1 = 39999;		// Set TOP count for timer1 in ICR1 register

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/8 
	F = 2MHz, T = 0.5us 
	For a period of 20ms, need 20ms/0.5us instructions = 40,000
	40,000 instructions per 20ms -> 2 instructions per 1us
	 */
	
	TCCR1A = (1<<COM1A1) | (1<<WGM11);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
	
	DDRB |= (1 << 1); //OC1A for ATmega328PB = PB1
	while(1)
	{
		//OCR1A will have a range of 1999 to 4999. The 3001HB Servo motor has a maximum travel length listed of approx. 165° from 800us -> 2200us
		//Was able to push to 2500us before the motor would stop turning and buzz
		OCR1A = 1999 + (adc_read()*2.93) ;	//Max adc value is 1023 and 3000/1023 ~= 2.93

	}
}

void adc_init() {
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
}

int adc_read()
{
	ADCSRA|=(1<<ADSC);	//start conversion
	while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
	
	ADCSRA |= (1<<ADIF);
	
	int tempADC = ADCL;
	tempADC = tempADC | (ADCH<<8);
	
	char output[20];
	snprintf(output, sizeof(output), "%d\r\n", tempADC); //prints out potentiometer value to serial terminal
	USART_putstring(output);
	
	return tempADC;
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
