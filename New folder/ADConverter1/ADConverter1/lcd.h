/*
 * lcd.h
 *
 * Created: 8/3/2023 9:14:38 PM
 *  Author: Acer
 */ 
#ifndef LCD_H_
#define LCD_H_

#define LcdDataBus    PORTK
#define LcdControlBus PORTK

#define LcdDataBusDirnReg DDRK

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void Lcd_CmdWrite(char cmd)
{
	LcdDataBus = (cmd & 0xF0);    //Set upper 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS);//Set RS pin to LOW
	LcdControlBus &= ~(1<<LCD_RW);//Set RW pin to LOW
	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms(10);
	LcdControlBus &= ~(1<<LCD_EN);
	
	_delay_ms(10);
	
	LcdDataBus = ((cmd<<4) & 0xF0);    //Set lower 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS);//Set RS pin to LOW
	LcdControlBus &= ~(1<<LCD_RW);//Set RW pin to LOW
	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms(10);
	LcdControlBus &= ~(1<<LCD_EN);
	_delay_ms(10);
}

void Lcd_DataWrite(char dat)
{
	LcdDataBus = (dat & 0xF0);    //Set upper 4 bits of the data
	LcdControlBus |= (1<<LCD_RS);//Set RS pin to High
	LcdControlBus &= ~(1<<LCD_RW);//Set RW pin to LOW
	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms(10);
	LcdControlBus &= ~(1<<LCD_EN);
	
	_delay_ms(10);
	
	LcdDataBus = ((dat<<4) & 0xF0);    //Set lower 4 bits of the data
	LcdControlBus |= (1<<LCD_RS);//Set RS pin to HIGH
	LcdControlBus &= ~(1<<LCD_RW);//Set RW pin to LOW
	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms(10);
	LcdControlBus &= ~(1<<LCD_EN);
	_delay_ms(10);
}
void Lcd_StringWrite(const char* str)
{
	while (*str) // Loop through the string until null terminator is found
	{
		Lcd_DataWrite(*str); // Display one character at a time
		str++; // Move to the next character in the string
	}
}

void Lcd_init(void)
{
	LcdDataBusDirnReg = 0xFF;	//LCD attached port as output
	Lcd_CmdWrite(0x02); //Initialize LCD in 4-bit mode
	Lcd_CmdWrite(0x28); //enable 5x7 mode for chars
	Lcd_CmdWrite(0x0E); //Display OFF, Cursor ON
	Lcd_CmdWrite(0x01); //Clear Display
}

void Lcd_DisplayInt(int value)
{
	int temp;
	Lcd_CmdWrite(0x01); // Clear Display

	if (value < 0)
	{
		Lcd_DataWrite('-');
		value = -value;
	}
	else if (value == 0)
	{
		Lcd_DataWrite('0');
		return;
	}

	temp = value / 10000;
	if (temp > 0)
		Lcd_DataWrite(temp + '0');

	value = value % 10000;
	temp = value / 1000;
	if (temp > 0 || value >= 1000)
		Lcd_DataWrite(temp + '0');

	value = value % 1000;
	temp = value / 100;
	if (temp > 0 || value >= 100)
		Lcd_DataWrite(temp + '0');

	value = value % 100;
	temp = value / 10;
	if (temp > 0 || value >= 10)
		Lcd_DataWrite(temp + '0');

	value = value % 10;
	Lcd_DataWrite(value + '0');
}

#endif /* LCD_H_ */
