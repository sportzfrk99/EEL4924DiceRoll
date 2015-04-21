//Filename: UF_LCD.c
//Author: Kyle Thornton
/*-------------LCD Wiring Instructions for ATMEGA324P-------------


		    PIN1:  Vss = GND
		    PIN2:  Vdd = +5V
		    PIN3:   Vo = Pot driven by +5V
		    PIN4:   RS = PA6 (Pin 34)
		    PIN5:  R/W = GND (Read-only)
		    PIN6:    E = PA7 (Pin 33)
                  PIN7:  DB4 = PA0 (Pin 40)
                  PIN8:  DB5 = PA1 (Pin 39)
                  PIN9:  DB6 = PA2 (Pin 38)
                  PIN10: DB7 = PA3 (Pin 37)

 

------------------CODE BY KYLE THORNTON----------------------------*/

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL






void lcd_command(char cmd)
{
	char temp = cmd;
	PORTA=0;

	_delay_ms(5);

	cmd = ( (cmd & 0xF0) >> 4) | 0x80;		//Write Upper Nibble (RS=0) E --> 1
	PORTA = cmd;
	_delay_ms(5);
	cmd ^= 0x80;							//E --> 0
	PORTA = cmd;

	_delay_ms(5);

	cmd = temp;
	cmd = ( (cmd & 0x0F) ) | 0x80;			//Write Lower Nibble (RS=0) E --> 1
	PORTA = cmd;
	_delay_ms(5);
	cmd ^= 0x80;							//E --> 0
	PORTA = cmd;

	_delay_ms(7);
}

void lcd_char(char data)
{
	char temp = data;
	PORTA = 0x40;

	_delay_ms(5);

	data = ( (data & 0xF0) >> 4) | 0xC0;	//Write Upper Nibble (RS=1) E --> 1
	PORTA = data;
	_delay_ms(5);
	data ^= 0x80;							//  E --> 0
	PORTA = data;

	_delay_ms(5);
	data = temp;

	data = ( (data & 0x0F) ) | 0xC0;		//Write Lower Nibble (RS=1) E --> 1
	PORTA = data;
	_delay_ms(5);
	data ^= 0x80;							//E --> 0
	PORTA = data;

	_delay_ms(7);
}

void lcd_string(char * data) {
	int i;
	i = 0;
	while(data[i] != '\0') {
		lcd_char(data[i]);
		i++;
	}
}

void lcd_num(int number) {
	int thousands = 0;
	int hundreds = 0;
	int tens = 0;
	int ones = 0;
	
	if(number < 10000) {
		thousands = number / 1000;
		number = number % 1000;
		hundreds = number / 100;
		number = number % 100;
		tens = number / 10;
		ones = number % 10;
		
		
		if(thousands == 0) {
			lcd_char('0');
		}
		else if(thousands == 1) {
			lcd_char('1');
		}
		else if(thousands == 2) {
			lcd_char('2');
		}
		else if(thousands == 3) {
			lcd_char('3');
		}
		else if(thousands == 4) {
			lcd_char('4');
		}
		else if(thousands == 5) {
			lcd_char('5');
		}
		else if(thousands == 6) {
			lcd_char('6');
		}
		else if(thousands == 7) {
			lcd_char('7');
		}
		else if(thousands == 8) {
			lcd_char('8');
		}
		else {
			lcd_char('9');
		}
		
		if(hundreds == 0) {
			lcd_char('0');
		}
		else if(hundreds == 1) {
			lcd_char('1');
		}
		else if(hundreds == 2) {
			lcd_char('2');
		}
		else if(hundreds == 3) {
			lcd_char('3');
		}
		else if(hundreds == 4) {
			lcd_char('4');
		}
		else if(hundreds == 5) {
			lcd_char('5');
		}
		else if(hundreds == 6) {
			lcd_char('6');
		}
		else if(hundreds == 7) {
			lcd_char('7');
		}
		else if(hundreds == 8) {
			lcd_char('8');
		}
		else {
			lcd_char('9');
		}
		
		if(tens == 0) {
			lcd_char('0');
		}
		else if(tens == 1) {
			lcd_char('1');
		}
		else if(tens == 2) {
			lcd_char('2');
		}
		else if(tens == 3) {
			lcd_char('3');
		}
		else if(tens == 4) {
			lcd_char('4');
		}
		else if(tens == 5) {
			lcd_char('5');
		}
		else if(tens == 6) {
			lcd_char('6');
		}
		else if(tens == 7) {
			lcd_char('7');
		}
		else if(tens == 8) {
			lcd_char('8');
		}
		else {
			lcd_char('9');
		}
		
		if(ones == 0) {
			lcd_char('0');
		}
		else if(ones == 1) {
			lcd_char('1');
		}
		else if(ones == 2) {
			lcd_char('2');
		}
		else if(ones == 3) {
			lcd_char('3');
		}
		else if(ones == 4) {
			lcd_char('4');
		}
		else if(ones == 5) {
			lcd_char('5');
		}
		else if(ones == 6) {
			lcd_char('6');
		}
		else if(ones == 7) {
			lcd_char('7');
		}
		else if(ones == 8) {
			lcd_char('8');
		}
		else {
			lcd_char('9');
		}
		
	}
	else {
		lcd_string("XXXX");
	}
}
void lcd_init()
{
	DDRA |= 0xCF;					//Change DDRA --> DDRB and PORTA --> PORTB if LCD attached to port B and so forth
	lcd_command(0x33);				//Initialize LCD Driver
	lcd_command(0x32);				//Four bit mode
	lcd_command(0x2C);				//2 Line Mode
	lcd_command(0x0C);				//Display On, Cursor Off, Blink Off  Change to 0x0F if cursor is desired
	lcd_command(0x01);				//Clear Screen, Cursor Home
}