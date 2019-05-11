/*
 * Task1.c
 *
 * Created: 5/10/2019 12:47:54 PM
 * Author : gausp
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "APDS9960_def.h"							/* Include APDS9960 register define file */
#include "i2c_master.h"							/* Include I2C Master header file */
#include "uart.h"							/* Include USART header file */

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

uint16_t Clear_data, Red_data, Green_data, Blue_data;


void init_uart(uint16_t baudrate){

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
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

void uart_putc(unsigned char c){

	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = c; // output character saved in c
}

void uart_puts(char *s){
	while(*s){
		uart_putc(*s);
		s++;
	}
}

void init_APDS9960(void){
	_delay_ms(150);										/* Power up time >100ms */
	i2c_start(APDS9960_WRITE); //
	i2c_write(APDS9960_ENABLE); // 
	i2c_write(0x00); //Turn off all features
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_ATIME);
	i2c_write(DEFAULT_ATIME); // Set default integration time
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_CONTROL); //Set default gain value
	i2c_write(DEFAULT_AGAIN);
	i2c_stop();
	
	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_ENABLE); 
	i2c_write((1 << POWER) | (1 << AMBIENT_LIGHT));		// 
	i2c_stop();

}

void getreading(void){

	i2c_start(APDS9960_WRITE);
	i2c_write(APDS9960_CDATAL); // set pointer
	i2c_stop();

	i2c_start(APDS9960_READ);
	Clear_data = (((int)i2c_read_ack()) | (int)i2c_read_ack() << 8);
	Red_data = (((int)i2c_read_ack()) | (int)i2c_read_ack() << 8);
	Green_data = (((int)i2c_read_ack()) | (int)i2c_read_ack() << 8);
	Blue_data = (((int)i2c_read_ack()) | (int)i2c_read_ack() << 8);
		
	i2c_stop();
}

int main(void){
	char buffer[20];
	//init_uart(9600);
	usart_init(); //manually set for 115200 BAUD rate as formula has too much error
	i2c_init();
	init_APDS9960();
	
	while(1){
		getreading();


		USART_SendString("\n-----------------------\n");

		sprintf(buffer,"Clear = %d, ",Clear_data);
		USART_SendString(buffer);
		
		sprintf(buffer,"Red = %d, ",Red_data);
		USART_SendString(buffer);
		
		sprintf(buffer,"Green = %d, ",Green_data);
		USART_SendString(buffer);
		
		sprintf(buffer,"Blue = %d\n",Blue_data);
		USART_SendString(buffer);

		USART_SendString("\n-----------------------\n");
		_delay_ms(1000);

		
		char thingSpeakUpdate[150];

		char setMUX[] = "AT+CIPMUX=0\r\n"; //Repeating the steps ESPlorer used
		USART_SendString(setMUX);
		_delay_ms(1000);

		char thingSpeakStart[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
		USART_SendString(thingSpeakStart);
		_delay_ms(1000); //add in delay to allow for proper interactions

		char thingSpeakSend[] = "AT+CIPSEND=119\r\n"; //saying we'll send more data than we actually will
		USART_SendString(thingSpeakSend);
		_delay_ms(1000);
		
		snprintf(thingSpeakUpdate, sizeof(thingSpeakUpdate), "GET https://api.thingspeak.com/update?api_key=P7JD9OICCNIR59PL&field1=%d&field2=%d&field3=%d&field4=%d\r\n", Clear_data, Red_data, Green_data, Blue_data);
		USART_SendString(thingSpeakUpdate);
		_delay_ms(1000);

		char thingSpeakClose[] = "AT+CIPCLOSE\r\n";
		USART_SendString(thingSpeakClose);
		_delay_ms(1000);
		
		_delay_ms(1500);	//delay of 15s
	}
	
	return 0;
}


