#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#include "twi_lcd.h" // Include the appropriate LCD interface header
#include "twi.h"     // Include the appropriate TWI interface header

void initHardware(void);
void initADC(void);
int readADC(uint8_t);
void initUSART(void);
void transmitUSART(unsigned char);
void configExternalInterrupt0(void);
void setLED(){PORTB |= 0B00000001;};
void clearLED(){PORTB &= 0B11111110;};



// Global variables
unsigned char letter = 'D';
volatile int flag = 0; // Use volatile for variables accessed in ISR

int main(void) {
	int adcValue;

	initHardware();
	initUSART();
	initADC();
	twi_init();
	twi_lcd_init();
	configExternalInterrupt0();


	while (1) {
		
		adcValue = readADC(0);

		twi_lcd_cmd(0x01); // Clear Display
		twi_lcd_cmd(0x80); // Set cursor to the beginning of the first line

		if (adcValue >= 0 && adcValue < 200) {
			letter = 'A';
			twi_lcd_msg("Emergency ");
			
			/*if (PIND & (1 << PD2) )
			{
				transmitUSART(letter);
				setLED();
				_delay_ms(10000);
				clearLED();
				_delay_ms(30000);
				
			}*/
			_delay_ms(10000);
			
	
			} else if (adcValue >= 200 && adcValue < 400) {
			letter = 'B';
			twi_lcd_msg("Need Medicine");
			
			/*if (PIND & (1 << PD2) )
			{
				transmitUSART(letter);
				setLED();
				_delay_ms(30000);
				clearLED();
				_delay_ms(30000);
				
			}*/
			_delay_ms(10000);
			
	
			} else if (adcValue >= 400 && adcValue < 600) {
			letter = 'C';
			twi_lcd_msg("Need Water");
			
			/*if (PIND & (1 << PD2) )
			{
				transmitUSART(letter);
				setLED();
				_delay_ms(30000);
				clearLED();
				_delay_ms(30000);
				
			}*/
			_delay_ms(10000);
	
			} else if (adcValue >= 600 && adcValue < 800) {
			letter = 'D';
			twi_lcd_msg("Food");
			
			/*if (PIND & (1 << PD2) )
			{
				transmitUSART(letter);
				setLED();
				_delay_ms(30000);
				clearLED();
				_delay_ms(30000);
				
			}*/
			_delay_ms(10000);
	
			} else {
			twi_lcd_msg("Out");
			
			/*if (PIND & (1 << PD2) )
			{
				transmitUSART(letter);
				setLED();
				_delay_ms(30000);
				clearLED();
				_delay_ms(30000);
				
			}*/
			_delay_ms(10000);
		
			_delay_ms(10000);
		}
		/*if (PIND & (1 << PD2) )
		{
			transmitUSART(letter);
			setLED();
			_delay_ms(30000);
			clearLED();
			_delay_ms(30000);
			
			}*/

		
		twi_lcd_cmd(0x01); // Clear Display
		
	}

	return 0;
}

void initHardware(void)
{
	DDRA = 0B00000000;
	DDRC = 0B11111111;
	DDRE &= ~(1 << PE0); // Set PE0 (ADC input) as input
	DDRB |= 0B00000001;
}

void initADC(void)
{
	ADCSRA |= (1 << ADEN);     // Enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128
	ADMUX |= (1 << REFS0);     // Set ADC reference voltage to AVcc
}

int readADC(uint8_t adc_input_pin)
{
	ADMUX = (ADMUX & 0xF8) | (adc_input_pin & 0x07); // Clear first 3 bits of ADMUX and set ADC input pin
	ADCSRA |= (1 << ADSC); // Start ADC conversion

	while (ADCSRA & (1 << ADSC)); // Wait for conversion to finish

	return ADC;
}

void initUSART(void)
{
	UBRR0L = (unsigned char)(UBRR_VALUE);
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);

	UCSR0B = (1 << TXEN0); // Enable USART transmitter
	UCSR0C = (3 << UCSZ00); // Set frame format: 8 data bits, no parity, 1 stop bit
}

void transmitUSART(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
	UDR0 = data; // Transmit data
}

void configExternalInterrupt0(void)
{
	EICRA |= 0x03;
	EIMSK |= 0x04;
	sei();
}

ISR(INT2_vect)
{
	transmitUSART(letter);
	setLED();
	_delay_ms(2000);
	clearLED();
	//_delay_ms(30000);
	

}

