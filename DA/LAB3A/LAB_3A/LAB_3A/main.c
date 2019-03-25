/*
 * LAB_3A.c
 *
 * Created: 3/24/2019 10:06:35 AM
 * Author : gausp
 */ 

#include <stdio.h>	//included for snprintf
#include <stdlib.h> //included for rand
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

//Declaration of global variables
volatile float randomFlt;
volatile int randomInt;
char String1[] = "The random integer is ";  //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text
char String2[] = ", and the random float is ";
char randomIntString[20];
char randomFltString[20];


int main(void) {
	USART_init();        //Call the USART initialization code
	
	TIMSK1 = (1 << OCIE1A);
	sei();
	
	OCR1A = 62499; //Using TCNT = clk*delay/prescaler - 1 to find OCR1A given clk = 16MHz, OCR1A was calculated to 62,499
	TCCR1A = 0; // COM1A/B Normal Operation, OC1A/B Disconnected
	TCCR1B = (1 << WGM12) | (1 << CS12); //WGM CTC Mode, Prescaler = 256
	
	while (1) {      //Infinite loop

	}

	return 0;
}

void USART_init(void) {

	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

unsigned char USART_receive(void) {

	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;

}

void USART_send( unsigned char data) {

	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr) {

	while (*StringPtr != 0x00) {
		USART_send(*StringPtr);
		StringPtr++;
	}

}

ISR (TIMER1_COMPA_vect) // timer1 compare interrupt
{
	randomInt = rand();
	randomFlt = (randomInt / 13); //Generate random integer then divide by 13 to get a different floating point number
	
	snprintf(randomIntString, sizeof(randomIntString), "%d", randomInt);
	snprintf(randomFltString, sizeof(randomFltString), "%f\r\n", randomFlt); //convert numbers to strings
	
	USART_putstring(String1);    //Pass the string to the USART_putstring function and sends it over the serial
	USART_putstring(randomIntString);
	USART_putstring(String2);
	USART_putstring(randomFltString);
}