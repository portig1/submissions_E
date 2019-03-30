
/*
 * The program reads the LM34 temperature of the MCU using ADC and sends it to the PC.
 * If you put your finger on the MCU, the number increases.
 *
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>		
#include <util/delay.h>

void usart_init ();
void usart_send (unsigned char ch);

int main (void)
{
	usart_init ();
	
   /** Setup and enable ADC **/
   ADMUX = (0<<REFS1)|    // Reference Selection Bits
   (1<<REFS0)|    // AVcc - external cap at AREF
   (0<<ADLAR)|    // ADC Left Adjust Result
   (1<<MUX2)|     // Analog Channel Selection Bits
   (0<<MUX1)|     // ADC4 (PC4 PIN27)
   (0<<MUX0);
   ADCSRA = (1<<ADEN)|    // ADC ENable
   (0<<ADSC)|     // ADC Start Conversion
   (0<<ADATE)|    // ADC Auto Trigger Enable
   (0<<ADIF)|     // ADC Interrupt Flag
   (0<<ADIE)|     // ADC Interrupt Enable
   (1<<ADPS2)|    // ADC Prescaler Select Bits
   (0<<ADPS1)|
   (1<<ADPS0);
	
	while (1)
	{
		ADCSRA|=(1<<ADSC);	//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);

		int a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0) * 5000/10;
		usart_send((a/100)+'0');
		a = a % 100;
		usart_send((a/10)+'0');
		a = a % 10;
		usart_send((a)+'0');
		usart_send('\r');
		
		_delay_ms(100);
	}
	return 0;
}


void usart_init (void)
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (unsigned char ch)
{
	while (! (UCSR0A & (1<<UDRE0))); 	//wait until UDR0 is empty
	UDR0 = ch;							//transmit ch
}

void usart_print(char* str)
{
	int i = 0;
	while(str[i] != 0)
		usart_send(str[i]);
}
