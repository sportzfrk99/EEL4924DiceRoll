//Filename	 : UF_LCD.h
//Author	 : Kyle Thornton

#ifndef __UF_LCD_H
#define __UF_LCD_H
void lcd_command(char);
void lcd_char(char);
void lcd_init();
void lcd_string(char * data);
void lcd_num(int number);

#endif
