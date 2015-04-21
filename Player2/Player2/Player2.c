/*
 * Player1.c
 *
 * Created: 4/13/2015 9:01:18 PM
 *  Author: sportzfrk
 */ 


#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "ks0108.h"
#include "font5x8.h"
#include "nordic.h"
#include "SPI.h"
#include "wireless.h"

static uint8_t * data;
uint8_t payloadw[5];
int diceheld[5];
int players;
int turns;
int maxturns = 13;
int roll;
int diceval[5];
int totalscore;
int upperscore[6];
int upperused[6];
int lowerscore[7];
int lowerused[7];
int yahtzeebonus;
int upperbonus;
int joker;


void setup_ui(void) {
	ks0108ClearScreen();
	ks0108DrawRect(0,0,24,24,BLACK);
	ks0108DrawRect(26,0,24,24,BLACK);
	ks0108DrawRect(52,0,24,24,BLACK);
	ks0108DrawRect(78,0,24,24,BLACK);
	ks0108DrawRect(103,0,24,24,BLACK);
	ks0108GotoXY(0,26);
}

void drawdice(int position, int number) {
	if(number == 1) {
		if(position == 1) {
			ks0108FillRect(10,11,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(36,11,3,3,BLACK);
		}
		else if(position == 3) {
			ks0108FillRect(62,11,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(88,11,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(113,11,3,3,BLACK);
		}
	}
	else if(number == 2) {
		if(position == 1) {
			ks0108FillRect(3,3,3,3,BLACK);
			ks0108FillRect(18,19,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(29,3,3,3,BLACK);
			ks0108FillRect(44,19,3,3,BLACK);
		}
		else if(position == 3) {
			ks0108FillRect(55,3,3,3,BLACK);
			ks0108FillRect(70,19,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(81,3,3,3,BLACK);
			ks0108FillRect(96,19,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(106,3,3,3,BLACK);
			ks0108FillRect(121,19,3,3,BLACK);
		}
	}
	else if(number == 3) {
		if(position == 1) {
			ks0108FillRect(3,3,3,3,BLACK);
			ks0108FillRect(10,11,3,3,BLACK);
			ks0108FillRect(18,19,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(29,3,3,3,BLACK);
			ks0108FillRect(36,11,3,3,BLACK);
			ks0108FillRect(44,19,3,3,BLACK);
			
		}
		else if(position == 3) {
			ks0108FillRect(55,3,3,3,BLACK);
			ks0108FillRect(62,11,3,3,BLACK);
			ks0108FillRect(70,19,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(81,3,3,3,BLACK);
			ks0108FillRect(88,11,3,3,BLACK);
			ks0108FillRect(96,19,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(106,3,3,3,BLACK);
			ks0108FillRect(113,11,3,3,BLACK);
			ks0108FillRect(121,19,3,3,BLACK);
		}
	}
	else if(number == 4) {
		if(position == 1) {
			ks0108FillRect(3,3,3,3,BLACK);
			ks0108FillRect(3,19,3,3,BLACK);
			ks0108FillRect(18,3,3,3,BLACK);
			ks0108FillRect(18,19,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(29,3,3,3,BLACK);
			ks0108FillRect(29,19,3,3,BLACK);
			ks0108FillRect(44,3,3,3,BLACK);
			ks0108FillRect(44,19,3,3,BLACK);
		}
		else if(position == 3) {
			ks0108FillRect(55,3,3,3,BLACK);
			ks0108FillRect(55,19,3,3,BLACK);
			ks0108FillRect(70,3,3,3,BLACK);
			ks0108FillRect(70,19,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(81,3,3,3,BLACK);
			ks0108FillRect(81,19,3,3,BLACK);
			ks0108FillRect(96,3,3,3,BLACK);
			ks0108FillRect(96,19,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(106,3,3,3,BLACK);
			ks0108FillRect(106,19,3,3,BLACK);
			ks0108FillRect(121,3,3,3,BLACK);
			ks0108FillRect(121,19,3,3,BLACK);
		}
	}
	else if(number == 5) {
		if(position == 1) {
			ks0108FillRect(3,3,3,3,BLACK);
			ks0108FillRect(3,19,3,3,BLACK);
			ks0108FillRect(10,11,3,3,BLACK);
			ks0108FillRect(18,3,3,3,BLACK);
			ks0108FillRect(18,19,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(29,3,3,3,BLACK);
			ks0108FillRect(29,19,3,3,BLACK);
			ks0108FillRect(36,11,3,3,BLACK);
			ks0108FillRect(44,3,3,3,BLACK);
			ks0108FillRect(44,19,3,3,BLACK);
		}
		else if(position == 3) {
			ks0108FillRect(55,3,3,3,BLACK);
			ks0108FillRect(55,19,3,3,BLACK);
			ks0108FillRect(62,11,3,3,BLACK);
			ks0108FillRect(70,3,3,3,BLACK);
			ks0108FillRect(70,19,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(81,3,3,3,BLACK);
			ks0108FillRect(81,19,3,3,BLACK);
			ks0108FillRect(88,11,3,3,BLACK);
			ks0108FillRect(96,3,3,3,BLACK);
			ks0108FillRect(96,19,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(106,3,3,3,BLACK);
			ks0108FillRect(106,19,3,3,BLACK);
			ks0108FillRect(113,11,3,3,BLACK);
			ks0108FillRect(121,3,3,3,BLACK);
			ks0108FillRect(121,19,3,3,BLACK);
		}
	}
	else if(number == 6) {
		if(position == 1) {
			ks0108FillRect(3,3,3,3,BLACK);
			ks0108FillRect(3,11,3,3,BLACK);
			ks0108FillRect(3,19,3,3,BLACK);
			ks0108FillRect(18,3,3,3,BLACK);
			ks0108FillRect(18,11,3,3,BLACK);
			ks0108FillRect(18,19,3,3,BLACK);
		}
		else if(position == 2) {
			ks0108FillRect(29,3,3,3,BLACK);
			ks0108FillRect(29,11,3,3,BLACK);
			ks0108FillRect(29,19,3,3,BLACK);
			ks0108FillRect(44,3,3,3,BLACK);
			ks0108FillRect(44,11,3,3,BLACK);
			ks0108FillRect(44,19,3,3,BLACK);
		}
		else if(position == 3) {
			ks0108FillRect(55,3,3,3,BLACK);
			ks0108FillRect(55,11,3,3,BLACK);
			ks0108FillRect(55,19,3,3,BLACK);
			ks0108FillRect(70,3,3,3,BLACK);
			ks0108FillRect(70,11,3,3,BLACK);
			ks0108FillRect(70,19,3,3,BLACK);
		}
		else if(position == 4) {
			ks0108FillRect(81,3,3,3,BLACK);
			ks0108FillRect(81,11,3,3,BLACK);
			ks0108FillRect(81,19,3,3,BLACK);
			ks0108FillRect(96,3,3,3,BLACK);
			ks0108FillRect(96,11,3,3,BLACK);
			ks0108FillRect(96,19,3,3,BLACK);
		}
		else if(position == 5) {
			ks0108FillRect(106,3,3,3,BLACK);
			ks0108FillRect(106,11,3,3,BLACK);
			ks0108FillRect(106,19,3,3,BLACK);
			ks0108FillRect(121,3,3,3,BLACK);
			ks0108FillRect(121,11,3,3,BLACK);
			ks0108FillRect(121,19,3,3,BLACK);
		}
	}
}

void cleardice(int position) {
	if(position == 1) {
		ks0108FillRect(1,1,22,22,WHITE);
	}
	else if(position == 2) {
		ks0108FillRect(27,1,22,22,WHITE);
	}
	else if(position == 3) {
		ks0108FillRect(53,1,22,22,WHITE);
	}
	else if(position == 4) {
		ks0108FillRect(79,1,22,22,WHITE);
	}
	else if(position == 5) {
		ks0108FillRect(104,1,22,22,WHITE);
	}
}

void ks0108PutNum(int number) {
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
		
		if(thousands != 0) {
			if(thousands == 1) {
				ks0108PutChar('1');
			}
			else if(thousands == 2) {
				ks0108PutChar('2');
			}
			else if(thousands == 3) {
				ks0108PutChar('3');
			}
			else if(thousands == 4) {
				ks0108PutChar('4');
			}
			else if(thousands == 5) {
				ks0108PutChar('5');
			}
			else if(thousands == 6) {
				ks0108PutChar('6');
			}
			else if(thousands == 7) {
				ks0108PutChar('7');
			}
			else if(thousands == 8) {
				ks0108PutChar('8');
			}
			else {
				ks0108PutChar('9');
			}
		}
		
		if(thousands != 0 && hundreds == 0) {
			ks0108PutChar('0');
		}
		else if(hundreds != 0) {
			if(hundreds == 1) {
				ks0108PutChar('1');
			}
			else if(hundreds == 2) {
				ks0108PutChar('2');
			}
			else if(hundreds == 3) {
				ks0108PutChar('3');
			}
			else if(hundreds == 4) {
				ks0108PutChar('4');
			}
			else if(hundreds == 5) {
				ks0108PutChar('5');
			}
			else if(hundreds == 6) {
				ks0108PutChar('6');
			}
			else if(hundreds == 7) {
				ks0108PutChar('7');
			}
			else if(hundreds == 8) {
				ks0108PutChar('8');
			}
			else {
				ks0108PutChar('9');
			}
		}
		
		if((thousands != 0 || hundreds !=0) && tens == 0) {
			ks0108PutChar('0');
		}
		else if(tens != 0) {
			if(tens == 1) {
				ks0108PutChar('1');
			}
			else if(tens == 2) {
				ks0108PutChar('2');
			}
			else if(tens == 3) {
				ks0108PutChar('3');
			}
			else if(tens == 4) {
				ks0108PutChar('4');
			}
			else if(tens == 5) {
				ks0108PutChar('5');
			}
			else if(tens == 6) {
				ks0108PutChar('6');
			}
			else if(tens == 7) {
				ks0108PutChar('7');
			}
			else if(tens == 8) {
				ks0108PutChar('8');
			}
			else {
				ks0108PutChar('9');
			}
		}
		
		if(ones == 0) {
			ks0108PutChar('0');
		}
		else if(ones == 1) {
			ks0108PutChar('1');
		}
		else if(ones == 2) {
			ks0108PutChar('2');
		}
		else if(ones == 3) {
			ks0108PutChar('3');
		}
		else if(ones == 4) {
			ks0108PutChar('4');
		}
		else if(ones == 5) {
			ks0108PutChar('5');
		}
		else if(ones == 6) {
			ks0108PutChar('6');
		}
		else if(ones == 7) {
			ks0108PutChar('7');
		}
		else if(ones == 8) {
			ks0108PutChar('8');
		}
		else {
			ks0108PutChar('9');
		}
	}
	else {
		ks0108PutChar('X');
	}
}

void MoveToRowGamePlay(int row) {
	if(row == 1) {
		ks0108GotoXY(0,26);
	}
	else if(row == 2) {
		ks0108GotoXY(0,34);
	}
	else if(row == 3) {
		ks0108GotoXY(0,42);
	}
	else {
		ks0108GotoXY(0,50);
	}
}
void ClearRowGamePlay(int row) {
	if(row == 1) {
		ks0108FillRect(0,26,127,7,WHITE);
	}
	else if(row == 2) {
		ks0108FillRect(0,34,127,7, WHITE);
	}
	else if(row == 3) {
		ks0108FillRect(0,42,127,7,WHITE);
	}
	else {
		ks0108FillRect(0,50,127,7,WHITE);
	}
}

void GetDiceHeld() {
	int cursor;
	int loop;
	ClearRowGamePlay(2);
	ClearRowGamePlay(3);
	ClearRowGamePlay(4);
	MoveToRowGamePlay(2);
	ks0108Puts_P(PSTR("Select dice to hold"));
	MoveToRowGamePlay(3);
	ks0108Puts_P(PSTR("  "));
	ks0108PutChar('1');
	ks0108Puts_P(PSTR("   "));
	ks0108PutChar('2');
	ks0108Puts_P(PSTR("   "));
	ks0108PutChar('3');
	ks0108Puts_P(PSTR("    "));
	ks0108PutChar('4');
	ks0108Puts_P(PSTR("   "));
	ks0108PutChar('5');
	MoveToRowGamePlay(4);
	ks0108Puts_P(PSTR("          OK"));
	ks0108InvertRect(11,41,6,8);
	if(diceheld[1] == 1) {
		ks0108InvertRect(35,41,6,8);
	}
	if(diceheld[2] == 1) {
		ks0108InvertRect(59,41,6,8);
	}
	if(diceheld[3] == 1) {
		ks0108InvertRect(89,41,6,8);
	}
	if(diceheld[4] == 1) {
		ks0108InvertRect(113,41,6,8);
	}
	loop = 1;
	cursor = 0;
	while(loop) {
		if((PIND & (1<<PD3)) == 0) {
			while((PIND & (1<<PD3)) == 0);
			if(cursor < 5) {
				if(diceheld[cursor] == 0) {
					diceheld[cursor] = 1;
				}
				else {
					diceheld[cursor] = 0;
				}
			}
			else {
				loop = 0;
			}
		}
		else if((PIND & (1<<PD4)) == 0) {
			while((PIND & (1<<PD4)) == 0);
			if(cursor < 5) {
				if(diceheld[cursor] == 0) {
					if(cursor == 0) {
						ks0108InvertRect(11,41,6,8);
					}
					else if(cursor == 1) {
						ks0108InvertRect(35,41,6,8);
					}
					else if(cursor == 2) {
						ks0108InvertRect(59,41,6,8);
					}
					else if(cursor == 3) {
						ks0108InvertRect(89,41,6,8);
					}
					else {
						ks0108InvertRect(113,41,6,8);
					}
					
				}
				cursor++;
			}
			else {
				ks0108InvertRect(59,50,12,7);
				cursor = 0;
			}
			if(cursor < 5) {
				if(diceheld[cursor] == 0) {
					if(cursor == 0) {
						ks0108InvertRect(11,41,6,8);
					}
					else if(cursor == 1) {
						ks0108InvertRect(35,41,6,8);
					}
					else if(cursor == 2) {
						ks0108InvertRect(59,41,6,8);
					}
					else if(cursor == 3) {
						ks0108InvertRect(89,41,6,8);
					}
					else {
						ks0108InvertRect(113,41,6,8);
					}
				}
			}
			else {
				ks0108InvertRect(59,50,12,7);
			}
		}
		else if((PIND & (1<<PD5)) == 0) {
			while((PIND & (1<<PD5)) == 0);
			if(cursor < 5) {
				if(diceheld[cursor] == 0) {
					if(cursor == 0) {
						ks0108InvertRect(11,41,6,8);
					}
					else if(cursor == 1) {
						ks0108InvertRect(35,41,6,8);
					}
					else if(cursor == 2) {
						ks0108InvertRect(59,41,6,8);
					}
					else if(cursor == 3) {
						ks0108InvertRect(89,41,6,8);
					}
					else {
						ks0108InvertRect(113,41,6,8);
					}
				}
				if(cursor == 0) {
					cursor = 5;
				}
				else {
					cursor--;
				}
			}
			else {
				ks0108InvertRect(59,50,12,7);
				cursor = 4;
			}
			if(cursor < 5) {
				if(diceheld[cursor] == 0) {
					if(cursor == 0) {
						ks0108InvertRect(11,41,6,8);
					}
					else if(cursor == 1) {
						ks0108InvertRect(35,41,6,8);
					}
					else if(cursor == 2) {
						ks0108InvertRect(59,41,6,8);
					}
					else if(cursor == 3) {
						ks0108InvertRect(89,41,6,8);
					}
					else {
						ks0108InvertRect(113,41,6,8);
					}
				}
			}
			else {
				ks0108InvertRect(59,50,12,7);
			}
		}
	}
	ClearRowGamePlay(3);
	ClearRowGamePlay(4);
}

void getDice(void) {
	payloadw[0] = 0x01;
	payloadw[1] = 0x01;
	payloadw[2] = 0x01;
	payloadw[3] = 0x01;
	payloadw[4] = 0x01;
	resetWireless();
	changeToTransmit();
	transmit_payload(payloadw);
	int loop;
	loop = 1;
	while(loop) {
		if((GetReg(STATUS) & (1<<5)) != 0) {
			loop = 0;
		}
		if((GetReg(STATUS) & (1<<4)) != 0) {
			resetWireless();
			transmit_payload(payloadw);
		}
	}
	resetWireless();
	changeToReceive();
	while((GetReg(STATUS) & (1<< 6)) == 0) {
		receieve_payload();
	}
	data = WriteToWireless(R, R_RX_PAYLOAD, data, 5);
	
	resetWireless();
	changeToTransmit();	
}

int checkValid(void) {
	int loop;
	int answer = 0;
	ClearRowGamePlay(2);
	MoveToRowGamePlay(2);
	ks0108Puts_P(PSTR("Is this good? Y N"));
	ks0108InvertRect(83,34,6,8);
	loop = 1;
	while(loop) {
		if((PIND & (1<<PD3)) == 0) {
			while((PIND & (1<<PD3)) == 0);
			if(answer == 0) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else if((PIND & (1<<PD4)) == 0) {
			while((PIND & (1<<PD4)) == 0);
			ks0108InvertRect(83,34,6,7);
			ks0108InvertRect(94,34,6,7);
			if(answer == 0) {
				answer = 1;
			}
			else {
				answer = 0;
			}	
		}
		else if((PIND & (1<<PD5)) == 0) {
			while((PIND & (1<<PD5)) == 0);
			ks0108InvertRect(83,34,6,7);
			ks0108InvertRect(94,34,6,7);
			if(answer == 0) {
				answer = 1;
			}
			else {
				answer = 0;
			}
		}
	}
}

void endTurn(void) {
	int loop;
	int totalscoretemp;
	loop = 1;
	payloadw[0] = 0x02;
	payloadw[3] = 0x02;
	payloadw[4] = 0x02;
	totalscoretemp = totalscore;
	totalscoretemp >>= 8;
	payloadw[1] = totalscoretemp;
	totalscoretemp = totalscore;
	totalscoretemp &= 0x00FF;
	payloadw[2] = (uint8_t)totalscoretemp;
	resetWireless();
	changeToTransmit();
	transmit_payload(payloadw);
	while(loop) {
		if((GetReg(STATUS) & (1<<5)) != 0) {
			loop = 0;
		}
		if((GetReg(STATUS) & (1<<4)) != 0) {
			resetWireless();
			transmit_payload(payloadw);
		}
	}
	resetWireless();
	changeToReceive();	
}
void waitforTurn(void) {
	resetWireless();
	changeToReceive();
	while((GetReg(STATUS) & (1<< 6)) == 0) {
		receieve_payload();
	}
	data = WriteToWireless(R, R_RX_PAYLOAD, data, 5);
	
	resetWireless();
	changeToTransmit();	
}
void MoveToRowScoring(int row) {
	if(row == 1) {
		ks0108GotoXY(0,0);
	}
	else if(row == 2) {
		ks0108GotoXY(0,8);
	}
	else if(row == 3) {
		ks0108GotoXY(0,16);
	}
	else if(row == 4) {
		ks0108GotoXY(0,24);
	}
	else if(row == 5) {
		ks0108GotoXY(0,32);
	}
	else if(row == 6) {
		ks0108GotoXY(0,40);
	}
	else if(row == 7) {
		ks0108GotoXY(0,48);
	}
	else {
		ks0108GotoXY(0,56);
	}
}

void setup_upper(void) {
	ks0108ClearScreen();
	MoveToRowScoring(1);
	ks0108Puts_P(PSTR("Upper Section"));
	MoveToRowScoring(2);
	ks0108Puts_P(PSTR("Ones               "));
	if(upperused[0] == 1) {
		ks0108PutNum(upperscore[0]);
	}
	MoveToRowScoring(3);
	ks0108Puts_P(PSTR("Twos               "));
	if(upperused[1] == 1) {
		ks0108PutNum(upperscore[1]);
	}
	MoveToRowScoring(4);
	ks0108Puts_P(PSTR("Threes             "));
	if(upperused[2] == 1) {
		ks0108PutNum(upperscore[2]);
	}
	MoveToRowScoring(5);
	ks0108Puts_P(PSTR("Fours              "));
	if(upperused[3] == 1) {
		ks0108PutNum(upperscore[3]);
	}
	MoveToRowScoring(6);
	ks0108Puts_P(PSTR("Fives              "));
	if(upperused[4] == 1) {
		ks0108PutNum(upperscore[4]);
	}
	MoveToRowScoring(7);
	ks0108Puts_P(PSTR("Sixes              "));
	if(upperused[5] == 1) {
		ks0108PutNum(upperscore[5]);
	}
	MoveToRowScoring(8);
	ks0108Puts_P(PSTR("Bonus              "));
	ks0108PutNum(upperbonus);
	
}

void ScoreUpperSection(int cursor) {
	int ones = 0;
	int twos = 0;
	int threes = 0;
	int fours = 0;
	int fives = 0;
	int sixes = 0;
	int testbonus = 0;
	for(int i = 0; i < 5; i++) {
		if(diceval[i] == 1) {
			ones++;
		}
		else if(diceval[i] == 2) {
			twos++;
		}
		else if(diceval[i] == 3) {
			threes++;
		}
		else if(diceval[i] == 4) {
			fours++;
		}
		else if(diceval[i] == 5) {
			fives++;
		}
		else {
			sixes++;
		}
	}
	
	if(cursor == 0) {
		upperscore[0] = ones * 1;
		upperused[0] = 1;
	}
	else if(cursor == 1) {
		upperscore[1] = twos * 2;
		upperused[1] = 1;
	}
	else if(cursor == 2) {
		upperscore[2] = threes * 3;
		upperused[2] = 1;
	}
	else if(cursor == 3) {
		upperscore[3] = fours * 4;
		upperused[3] = 1;
	}
	else if(cursor == 4) {
		upperscore[4] = fives * 5;
		upperused[4] = 1;
	}
	else {
		upperscore[5] = sixes * 6;
		upperused[5] = 1;
	}
	testbonus = upperscore[0] + upperscore[1] + upperscore[2] + upperscore[3] + upperscore[4] + upperscore[5];
	if(testbonus >= 63) {
		upperbonus = 35;
	}
	else {
		upperbonus = 0;
	}
}

void setup_lower(void) {
	ks0108ClearScreen();
	MoveToRowScoring(1);
	ks0108Puts_P(PSTR("Lower Section"));
	MoveToRowScoring(2);
	ks0108Puts_P(PSTR("3 of a Kind        "));
	if(lowerused[0] == 1) {
		ks0108PutNum(lowerscore[0]);
	}
	MoveToRowScoring(3);
	ks0108Puts_P(PSTR("4 of a Kind        "));
	if(lowerused[1] == 1) {
		ks0108PutNum(lowerscore[1]);
	}
	MoveToRowScoring(4);
	ks0108Puts_P(PSTR("Full House         "));
	if(lowerused[2] == 1) {
		ks0108PutNum(lowerscore[2]);
	}
	MoveToRowScoring(5);
	ks0108Puts_P(PSTR("Small Straight     "));
	if(lowerused[3] == 1) {
		ks0108PutNum(lowerscore[3]);
	}
	MoveToRowScoring(6);
	ks0108Puts_P(PSTR("Large Straight     "));
	if(lowerused[4] == 1) {
		ks0108PutNum(lowerscore[4]);
	}
	MoveToRowScoring(7);
	ks0108Puts_P(PSTR("Yahtzee            "));
	if(lowerused[5] == 1) {
		ks0108PutNum(lowerscore[5]);
	}
	MoveToRowScoring(8);
	ks0108Puts_P(PSTR("Chance             "));
	if(lowerused[6] == 1) {
		ks0108PutNum(lowerscore[6]);
	}
}

void ScoreLowerSection(int cursor) {
	int ones = 0;
	int twos = 0;
	int threes = 0;
	int fours = 0;
	int fives = 0;
	int sixes = 0;
	for(int i = 0; i < 5; i++) {
		if(diceval[i] == 1) {
			ones++;
		}
		else if(diceval[i] == 2) {
			twos++;
		}
		else if(diceval[i] == 3) {
			threes++;
		}
		else if(diceval[i] == 4) {
			fours++;
		}
		else if(diceval[i] == 5) {
			fives++;
		}
		else {
			sixes++;
		}
	}
	
	if(cursor == 0) {
		if((ones >= 3) || (twos >= 3) || (threes >= 3) || (fours >= 3) || (fives >= 3) || (sixes >= 3)) {
			lowerscore[0] = diceval[0] + diceval[1] + diceval[2] + diceval[3] + diceval[4];
		}
		else {
			lowerscore[0] = 0;
		}
		lowerused[0] = 1;
	}
	else if(cursor == 1) {
		if((ones >= 4) || (twos >= 4) || (threes >= 4) || (fours >= 4) || (fives >= 4) || (sixes >= 4)) {
			lowerscore[1] = diceval[0] + diceval[1] + diceval[2] + diceval[3] + diceval[4];
		}
		else {
			lowerscore[1] = 0;
		}
		lowerused[1] = 1;
	}
	else if(cursor == 2) {
		if((ones == 3) || (twos == 3) || (threes == 3) || (fours == 3) || (fives == 3) || (sixes == 3)) {
			if((ones == 2) || (twos == 2) || (threes == 2) || (fours == 2) || (fives == 2) || (sixes == 2)) {
				lowerscore[2] = 25;
			}
			else {
				lowerscore[2] = 0;
			}
		}
		else if(joker == 1) {
			lowerscore[2] = 25;
			joker = 0;
		}
		else {
			lowerscore[2] = 0;
		}
		lowerused[2] = 1;
	}
	else if(cursor == 3) {
		if(((ones >= 1) && (twos >= 1) && (threes >= 1) && (fours >= 1)) || ((twos >= 1) && (threes >= 1) && (fours >= 1) && (fives >= 1)) || ((threes >= 1) && (fours >= 1) && (fives >= 1) && (sixes >= 1))) {
			lowerscore[3] = 30;
		}
		else if(joker == 1) {
			lowerscore[3] = 30;
			joker = 0;
		}
		else {
			lowerscore[3] = 0;
		}
		lowerused[3] = 1;
	}
	else if(cursor == 4) {
		if(((ones == 1) && (twos == 1) && (threes == 1) && (fours == 1) && (fives == 1)) || ((twos == 1) && (threes == 1) && (fours == 1) && (fives == 1) && (sixes == 1))) {
			lowerscore[4] = 40;
		}
		else if(joker == 1) {
			lowerscore[4] = 40;
			joker = 0;
		}
		else {
			lowerscore[4] = 0;
		}
		lowerused[4] = 1;
	}
	else if(cursor == 5) {
		if((ones == 5) || (twos == 5) || (threes == 5) || (fours == 5) || (fives == 5) || (sixes == 5)) {
			lowerscore[5] = 50;
		}
		else {
			lowerscore[5] = 0;
		}
		lowerused[5] = 1;
	}
	
	else {
		lowerscore[6] = diceval[0] + diceval[1] + diceval[2] + diceval[3] + diceval[4];
		lowerused[6] = 1;
	}
}

void scoreDice(void) {
	int skipscore = 0;
	int outerloop = 1;
	int innerloop = 1;
	int section = 0;
	int cursor = 0;
	joker = 0;
	int ones = 0;
	int twos = 0;
	int threes = 0;
	int fours = 0;
	int fives = 0;
	int sixes = 0;
	for(int i = 0; i < 5; i++) {
		if(diceval[i] == 1) {
			ones++;
		}
		else if(diceval[i] == 2) {
			twos++;
		}
		else if(diceval[i] == 3) {
			threes++;
		}
		else if(diceval[i] == 4) {
			fours++;
		}
		else if(diceval[i] == 5) {
			fives++;
		}
		else {
			sixes++;
		}
	}
	if((ones == 5) || (twos == 5) || (threes == 5) || (fours == 5) || (fives == 5) || (sixes == 5)) {
		if(lowerused[5] == 1) {
			section = 2;
			ks0108ClearScreen();
			ks0108GotoXY(0,0);
			if(lowerscore[5] == 50) {
				yahtzeebonus += 100;
				ks0108Puts_P(PSTR("100 point bonus!\n"));
			}
			if(ones == 5 && upperused[0] == 0) {
				upperscore[0] = 5;
				upperused[0] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 1s"));
			}
			else if(twos == 5 && upperused[1] == 0) {
				upperscore[1] = 10;
				upperused[1] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 2s"));
			}
			else if(threes == 5 && upperused[2] == 0) {
				upperscore[2] = 15;
				upperused[2] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 3s"));
			}
			else if(fours == 5 && upperused[3] == 0) {
				upperscore[3] = 20;
				upperused[3] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 4s"));
			}
			else if(fives == 5 && upperused[4] == 0) {
				upperscore[4] = 25;
				upperused[4] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 5s"));
			}
			else if(sixes == 5 && upperused[5] == 0) {
				upperscore[5] = 30;
				upperused[5] = 1;
				skipscore = 1;
				ks0108Puts_P(PSTR("Force score into 6s"));
			}
			else {
				joker = 1;
				ks0108Puts_P(PSTR("Use a joker this turn"));
			}
			int loop = 1;
			while(loop) {
				if((PIND & (1<<PD3)) == 0) {
					while((PIND & (1<<PD3)) == 0);
					loop = 0;
				}
			}
		}
	}
	
	if(!skipscore) {	
		while(outerloop) {
			if(section == 0) {
				setup_upper();
			}
			else {
				setup_lower();
			}
			ks0108InvertRect(113,7,12,7);
			innerloop = 1;
			while(innerloop) {
				if((PIND & (1<<PD3)) == 0) {
					while((PIND & (1<<PD3)) == 0);
					if(section == 0) {
						if(cursor < 6) {
							if(upperused[cursor] == 0) {
								innerloop = 0;
								outerloop = 0;
							}
						}
					}
					else {
						if(lowerused[cursor] == 0) {
							innerloop = 0;
							outerloop = 0;
						}
					}
				}
				else if((PIND & (1<<PD7)) == 0) {
					while((PIND & (1<<PD7)) == 0);
				
					if(cursor == 0) {
						ks0108InvertRect(113,7,12,7);
					}
					else if(cursor == 1) {
						ks0108InvertRect(113,15,12,7);
					}
					else if(cursor == 2) {
						ks0108InvertRect(113,23,12,7);
					}
					else if(cursor == 3) {
						ks0108InvertRect(113,31,12,7);
					}
					else if(cursor == 4) {
						ks0108InvertRect(113,39,12,7);
					}
					else if(cursor == 5) {
						ks0108InvertRect(113,47,12,7);
					}
					else {
						ks0108InvertRect(113,55,12,7);
					}
				
					if(cursor < 6) {
						cursor++;
					}
					else {
						cursor = 0;
					}
				
					if(cursor == 0) {
						ks0108InvertRect(113,7,12,7);
					}
					else if(cursor == 1) {
						ks0108InvertRect(113,15,12,7);
					}
					else if(cursor == 2) {
						ks0108InvertRect(113,23,12,7);
					}
					else if(cursor == 3) {
						ks0108InvertRect(113,31,12,7);
					}
					else if(cursor == 4) {
						ks0108InvertRect(113,39,12,7);
					}
					else if(cursor == 5) {
						ks0108InvertRect(113,47,12,7);
					}
					else {
						ks0108InvertRect(113,55,12,7);
					}
				}
				else if((PIND & (1<<PD6)) == 0) {
					while((PIND & (1<<PD6)) == 0);
				
					if(cursor == 0) {
						ks0108InvertRect(113,7,12,7);
					}
					else if(cursor == 1) {
						ks0108InvertRect(113,15,12,7);
					}
					else if(cursor == 2) {
						ks0108InvertRect(113,23,12,7);
					}
					else if(cursor == 3) {
						ks0108InvertRect(113,31,12,7);
					}
					else if(cursor == 4) {
						ks0108InvertRect(113,39,12,7);
					}
					else if(cursor == 5) {
						ks0108InvertRect(113,47,12,7);
					}
					else {
						ks0108InvertRect(113,55,12,7);
					}
				
					if(cursor == 0) {
						cursor = 6;
					}
					else {
						cursor--;
					}
				
					if(cursor == 0) {
						ks0108InvertRect(113,7,12,7);
					}
					else if(cursor == 1) {
						ks0108InvertRect(113,15,12,7);
					}
					else if(cursor == 2) {
						ks0108InvertRect(113,23,12,7);
					}
					else if(cursor == 3) {
						ks0108InvertRect(113,31,12,7);
					}
					else if(cursor == 4) {
						ks0108InvertRect(113,39,12,7);
					}
					else if(cursor == 5) {
						ks0108InvertRect(113,47,12,7);
					}
					else {
						ks0108InvertRect(113,55,12,7);
					}
				}
				else if((PIND & (1<<PD4)) == 0) {
					while((PIND & (1<<PD4)) == 0);
					if(section == 0) {
						section = 1;
					}
					else {
						section = 0;
					}
					innerloop = 0;
					cursor = 0;
				}
				else if((PIND & (1<<PD5)) == 0) {
					while((PIND & (1<<PD5)) == 0);
					if(section == 0) {
						section = 1;
					}
					else {
						section = 0;
					}
					innerloop = 0;
					cursor = 0;
				}
			}
		}
		if(section == 0) {
			ScoreUpperSection(cursor);
		}
	
		else if(section == 1) {
			ScoreLowerSection(cursor);
		}
	}
}

int main(void) {
	totalscore = 0;
	upperscore[0] = 0;
	upperscore[1] = 0;
	upperscore[2] = 0;
	upperscore[3] = 0;
	upperscore[4] = 0;
	upperscore[5] = 0;
	upperused[0] = 0;
	upperused[1] = 0;
	upperused[2] = 0;
	upperused[3] = 0;
	upperused[4] = 0;
	upperused[5] = 0;
	lowerscore[0] = 0;
	lowerscore[1] = 0;
	lowerscore[2] = 0;
	lowerscore[3] = 0;
	lowerscore[4] = 0;
	lowerscore[5] = 0;
	lowerscore[6] = 0;
	lowerused[0] = 0;
	lowerused[1] = 0;
	lowerused[2] = 0;
	lowerused[3] = 0;
	lowerused[4] = 0;
	lowerused[5] = 0;
	lowerused[6] = 0;
	upperbonus = 0;
	yahtzeebonus = 0;
	int loop;
	loop = 1;
	_delay_ms(500);
	ks0108Init(0);
	initSPI();
	initWireless();
	ks0108SelectFont(monospace, ks0108ReadFontData, BLACK);
	DDRD = 0x00;
	PORTD = 0x00;
	ks0108Puts_P(PSTR("Waiting for Turn"));
	for(turns = 0; turns < maxturns; turns++) {
		waitforTurn();
		setup_ui();
		diceheld[0] = 0;
		diceheld[1] = 0;
		diceheld[2] = 0;
		diceheld[3] = 0;
		diceheld[4] = 0;
		diceval[0] = 0;
		diceval[1] = 0;
		diceval[2] = 0;
		diceval[3] = 0;
		diceval[4] = 0;
		for(int j = 0; j < 3; j++) {
			ClearRowGamePlay(1);
			MoveToRowGamePlay(1);
			ks0108Puts_P(PSTR("Turn "));
			ks0108PutNum(turns+1);
			ks0108Puts_P(PSTR(" Roll "));
			ks0108PutNum(j+1);
			if(j != 0) {	
				for(int l = 0; l < 5; l++) {
					if(diceheld[l] == 0) {
						cleardice(l+1);
					}
				}
			}
			for(int k = 0; k < 5; k++) {
				if(diceheld[k] == 0) {
					int good;
					good = 0;
					while(!good) {
						cleardice(k+1);
						ClearRowGamePlay(2);
						MoveToRowGamePlay(2);
						ks0108Puts_P(PSTR("Press Center to Scan"));
						loop = 1;
						while(loop) {
							if((PIND & (1<<PD3)) == 0) {
								loop = 0;
								while((PIND & (1<<PD3)) == 0);
							}
						}
						getDice();
						diceval[k] = data[0];
						drawdice(k+1,data[0]);
						good = checkValid();
					}
				}
			}
			if((j == 0) || (j == 1 && (diceheld[0] == 0 || diceheld[1] == 0 || diceheld[2] == 0 || diceheld[3] == 0 || diceheld[4] == 0))) {
				GetDiceHeld();
			}
		}
		scoreDice();
		totalscore = upperscore[0] + upperscore[1] + upperscore[2] + upperscore[3] + upperscore[4] + upperscore[5] + upperbonus + lowerscore[0] + lowerscore[1] + lowerscore[2] + lowerscore[3] + lowerscore[4] + lowerscore[5] + lowerscore[6] + yahtzeebonus;
		ks0108ClearScreen();
		ks0108GotoXY(0,0);
		ks0108Puts_P(PSTR("Total Score "));
		ks0108PutNum(totalscore);
		loop = 1;
		while(loop) {
			if((PIND & (1<<PD3)) == 0) {
				while((PIND & (1<<PD3)) == 0);
				loop = 0;
			}
		}
		ks0108FillRect(0,0,127,8,WHITE);
		ks0108GotoXY(0,0);
		ks0108Puts_P(PSTR("Waiting for turn"));
		endTurn();
	}
	while((GetReg(STATUS) & (1<< 6)) == 0) {
		receieve_payload();
	}
	data = WriteToWireless(R, R_RX_PAYLOAD, data, 5);
	resetWireless();
	ks0108ClearScreen();
	ks0108GotoXY(0,0);
	if(data[0] == 0) {
		ks0108Puts_P(PSTR("You lose\n"));
	}
	else if(data[0] == 1) {
		ks0108Puts_P(PSTR("You win!\n"));
	}
	else {
		ks0108Puts_P(PSTR("You tie\n"));
	}
	ks0108Puts_P(PSTR("Final score: "));
	ks0108PutNum(totalscore);
	while(1);
}