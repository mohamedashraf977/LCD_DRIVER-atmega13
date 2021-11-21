/*
 * main.c
 *
 * Created: 21-Nov-21 3:56:53 PM
 *  Author: Mohamed Ashraf
 */ 


#include "LCD_DRIVER.h"


void LCD_sendCommand (uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT=command;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	
}

void LCD_init ()
{
	LCD_DATA_PORT_DIR=0xFF;
	LCD_DATA_PORT=0x00;
	
	SET_BIT(LCD_CTRL_PORT_DIR,RS);
	SET_BIT(LCD_CTRL_PORT_DIR,RW);
	SET_BIT(LCD_CTRL_PORT_DIR,E);
	
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	CLEAR_BIT(LCD_CTRL_PORT,E); 
	
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
		
	LCD_sendCommand(CURSOR_OFF); /* cursor off */
		
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
	
}


void LCD_displayCharacter (uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
}



void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

void LCD_intgerToString(int data)
{
	char buff[16]; /* String to hold the ascii result */
	itoa(data,buff,10); /* 10 for decimal */
	LCD_displayString(buff);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); //clear display
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	
	/* first of all calculate the required address */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}					
	/* to write to a specific address in the LCD 
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION); 
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}
