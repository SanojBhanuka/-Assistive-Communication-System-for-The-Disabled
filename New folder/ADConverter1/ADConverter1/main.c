/*
 * ADConverter1.c
 *
 * Created: 2023-06-25 17.31.35
 * Author : asus
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#include "lcd.h"

void initHardware(void);
void initADC(void);
int readADC(uint8_t);
void configExternalInterrupt0(void);
void setLED(){ PORTC |= 0B00000001;}
	
void initUSART(void){
	UBRR0L = (unsigned char)(UBRR_VALUE);
	UBRR0H = (unsigned char)(UBRR_VALUE>>8);
	
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= (3<<UCSZ00);
}
	
void transmitUSART(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
//volatile char num = 0x30;

int main(void)
{
	int adcValue;
	initHardware();
	initUSART();
	initADC();
	Lcd_init();
	configExternalInterrupt0();
	
	/* Replace with your application code */
	while (1)
	{
		adcValue = readADC(0);
		
		Lcd_CmdWrite(0x01); // Clear Display
		Lcd_DataWrite("Pot Value: ");
		Lcd_DisplayInt(adcValue); // Display the potentiometer value
		
		// Set cursor to the beginning of the second line
		Lcd_CmdWrite(0xC0);
		
		if (adcValue >= 0 && adcValue < 100)
		{
			//num = '1';
			Lcd_StringWrite("Emergency Situation"); // Display instruction for the first range (0-100 ohms)
		}
		else if (adcValue >= 100 && adcValue < 200)
		{
			//num = 2;
			Lcd_StringWrite("Need Medicine"); // Display instruction for the second range (100-200 ohms)
		}
		else if (adcValue >= 200 && adcValue < 300)
		{
			//num = 3;
			Lcd_StringWrite("Need Water"); // Display instruction for the third range (200-300 ohms)
		}
		else if (adcValue >= 300 && adcValue < 400)
		{
			//num = 4;
			Lcd_StringWrite("Need food"); // Display instruction for the third range (200-300 ohms)
		}
		// Add more conditions for other value ranges and their corresponding instructions here...
		else
		{
			//num = 5;
			Lcd_StringWrite("Out of Range"); // Display a message if the value is outside the specified ranges
		}
		
		_delay_ms(2000);
	}
	return 0;
}


void initHardware(void){
	DDRA = 0B00000000;
	DDRC = 0B11111111;
	DDRE = 0B11111110;
	//DDRF = 0B00000000;
}

void initADC(void){
	ADCSRA |= (1<<ADEN); // ENABLE ADC
	ADCSRA |= 0B00000111; //PRESCALAR 128
	ADCSRA |= (1<<ADSC); // START THE CONVERSION
}

int readADC(uint8_t adc_input_pin){
	//ADMUX & 0xF0 - This clear the first 4 bits of ADMUX register
	ADMUX = (ADMUX & 0xF0) | adc_input_pin; // 0xF0 = 1111 0000
	ADMUX &= 0B00111111; // turning off the internal reference voltage
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

void configExternalInterrupt0(void)
{
	EICRA |= 0x03;
	EIMSK |= 0x01;
	sei();
}



ISR(INT0_vect)
{
	setLED();
	transmitUSART("G");
}
