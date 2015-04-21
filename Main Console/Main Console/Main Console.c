/*
 * Main_Console.c
 *
 * Created: 4/13/2015 10:17:34 PM
 *  Author: sportzfrk
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "UF_LCD.h"
#include "wireless.h"
#include "nordic.h"
#include "SPI.h"

// Are you using an SPI interface?  if so, uncomment this line
#define SPI

#define PIXY_ARRAYSIZE              100
#define PIXY_START_WORD             0xaa55
#define PIXY_START_WORD_CC          0xaa56
#define PIXY_START_WORDX            0x55aa
#define PIXY_SERVO_SYNC             0xff
#define PIXY_CAM_BRIGHTNESS_SYNC    0xfe
#define PIXY_LED_SYNC               0xfd
#define PIXY_OUTBUF_SIZE            64

#define PIXY_SYNC_BYTE              0x5a
#define PIXY_SYNC_BYTE_DATA         0x5b

static uint8_t * data;
// the routines
uint8_t USART_Trans(uint8_t data);

// data types
typedef enum
{
	NORMAL_BLOCK,
	CC_BLOCK // color code block
} BlockType;

typedef struct
{
	uint16_t signature;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t angle; // angle is only available for color coded blocks
} Block;

// communication routines


#ifndef SPI //////////// for I2C and UART

extern uint8_t getByte(void);
extern int sendByte(uint8_t byte);

uint16_t getWord(void)
{
	// this routine assumes little endian
	uint16_t w;
	uint8_t c;
	c = readByte();
	w = readByte();
	w <<= 8;
	w |= c;
	return w;
}

int send(uint8_t *data, int len)
{
	int i;
	for (i=0; i<len; i++)
	sendByte(data[i]);

	return len;
}

#else ///////////// SPI routines

// SPI sends as it receives so we need a getByte routine that
// takes an output data argument
static uint16_t getWord(void);
static int send(uint8_t *data, int len);
// variables for a little circular queue
static uint8_t g_outBuf[PIXY_OUTBUF_SIZE];
static uint8_t g_outLen = 0;
static uint8_t g_outWriteIndex = 0;
static uint8_t g_outReadIndex = 0;

uint16_t getWord()
{
	// ordering is big endian because Pixy is sending 16 bits through SPI
	uint16_t w;
	uint8_t c, cout = 0;

	if (g_outLen)
	{
		w = USART_Trans(PIXY_SYNC_BYTE_DATA);
		cout = g_outBuf[g_outReadIndex++];
		g_outLen--;
		if (g_outReadIndex==PIXY_OUTBUF_SIZE)
		g_outReadIndex = 0;
	}
	else
	w = USART_Trans(PIXY_SYNC_BYTE); // send out sync byte
	w <<= 8;
	c = USART_Trans(cout); // send out data byte
	w |= c;

	return w;
}

int send(uint8_t *data, int len)
{
	int i;

	// check to see if we have enough space in our circular queue
	if (g_outLen+len>PIXY_OUTBUF_SIZE)
	return -1;

	g_outLen += len;
	for (i=0; i<len; i++)
	{
		g_outBuf[g_outWriteIndex++] = data[i];
		if (g_outWriteIndex==PIXY_OUTBUF_SIZE)
		g_outWriteIndex = 0;
	}
	return len;
}

#endif //////////////// end SPI routines

static int g_skipStart = 0;
static BlockType g_blockType;
static Block *g_blocks;

void init()
{
	g_blocks = (Block *)malloc(sizeof(Block)*PIXY_ARRAYSIZE);
}

int getStart(void)
{
	uint16_t w, lastw;

	lastw = 0xffff;

	while(1)
	{
		w = getWord();
		if (w==0 && lastw==0)
		return 0; // no start code
		else if (w==PIXY_START_WORD && lastw==PIXY_START_WORD)
		{
			g_blockType = NORMAL_BLOCK;
			return 1; // code found!
		}
		else if (w==PIXY_START_WORD_CC && lastw==PIXY_START_WORD)
		{
			g_blockType = CC_BLOCK; // found color code block
			return 1;
		}
		else if (w==PIXY_START_WORDX)
		#ifdef SPI
		USART_Trans(0); // we're out of sync! (backwards)
		#else
		USART_Trans(); // we're out of sync! (backwards)
		#endif
		lastw = w;
	}
}

uint16_t getBlocks(uint16_t maxBlocks)
{
	uint8_t i;
	uint16_t w, blockCount, checksum, sum;
	Block *block;

	if (!g_skipStart)
	{
		if (getStart()==0)
		return 0;
	}
	else
	g_skipStart = 0;

	for(blockCount=0; blockCount<maxBlocks && blockCount<PIXY_ARRAYSIZE;)
	{
		checksum = getWord();
		if (checksum==PIXY_START_WORD) // we've reached the beginning of the next frame
		{
			g_skipStart = 1;
			g_blockType = NORMAL_BLOCK;
			return blockCount;
		}
		else if (checksum==PIXY_START_WORD_CC)
		{
			g_skipStart = 1;
			g_blockType = CC_BLOCK;
			return blockCount;
		}
		else if (checksum==0)
		return blockCount;

		block = g_blocks + blockCount;

		for (i=0, sum=0; i<sizeof(Block)/sizeof(uint16_t); i++)
		{
			if (g_blockType==NORMAL_BLOCK && i>=5) // no angle for normal block
			{
				block->angle = 0;
				break;
			}
			w = getWord();
			sum += w;
			*((uint16_t *)block + i) = w;
		}

		// check checksum
		if (checksum==sum)
		blockCount++;
		else
		printf("checksum error!\n");

		w = getWord();
		if (w==PIXY_START_WORD)
		g_blockType = NORMAL_BLOCK;
		else if (w==PIXY_START_WORD_CC)
		g_blockType = CC_BLOCK;
		else
		return blockCount;
	}
}

int setServos(uint16_t s0, uint16_t s1)
{
	uint8_t outBuf[6];

	outBuf[0] = 0x00;
	outBuf[1] = PIXY_SERVO_SYNC;
	*(uint16_t *)(outBuf + 2) = s0;
	*(uint16_t *)(outBuf + 4) = s1;

	return send(outBuf, 6);
}

int setBrightness(uint8_t brightness)
{
	uint8_t outBuf[3];

	outBuf[0] = 0x00;
	outBuf[1] = PIXY_CAM_BRIGHTNESS_SYNC;
	outBuf[2] = brightness;

	return send(outBuf, 3);
}

int setLED(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t outBuf[5];

	outBuf[0] = 0x00;
	outBuf[1] = PIXY_LED_SYNC;
	outBuf[2] = r;
	outBuf[3] = g;
	outBuf[4] = b;

	return send(outBuf, 5);
}

void USART_SPI_Init(void) {
	UBRR1 = 0;
	DDRD |= (1<<DDD4);
	UCSR1C = 0xC0;
	UCSR1B = 0x18;
	UBRR1 = 3;
}

uint8_t USART_Trans(uint8_t data) {
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = data;
	while(!(UCSR1A & (1<<RXC1)));
	return UDR1;
}

int getDice(void) {
	int i;
	uint16_t blocks;
	uint16_t pixymax[7];
	pixymax[0] = 0;
	pixymax[1] = 0;
	pixymax[2] = 0;
	pixymax[3] = 0;
	pixymax[4] = 0;
	pixymax[5] = 0;
	pixymax[6] = 0;
	int max;
	for(i=0; i<1000; i++) {
		blocks = getBlocks(9);
		if(blocks !=0) {
			if(g_blocks[0].signature == 1) {
				pixymax[1] += 1;
			}
			else if(g_blocks[0].signature == 2) {
				pixymax[2] += 1;
			}
			else if(g_blocks[0].signature == 3) {
				pixymax[3] += 1;
			}
			else if(g_blocks[0].signature == 4) {
				pixymax[4] += 1;
			}
			else if(g_blocks[0].signature == 5) {
				pixymax[5] += 1;
			}
			else if(g_blocks[0].signature == 6) {
				pixymax[6] += 1;
			}
			else {
				pixymax[0] += 1;
			}
		}
		else {
			pixymax[0] += 1;
		}
	}
	max = 1;
	for(i=2; i <7; i++) {
		if(pixymax[i] >= pixymax[max]) {
			max = i;
		}
	}
	return max;
}

void changeAddress(int player) {
		_delay_ms(100);
		uint8_t val[5];
		int i;
		for(i=0; i<5; i++) {
			if(player == 1){
				val[i] = 0x13;
			}
			else {
				val[i] = 0x12;
			}
		}
		WriteToWireless(W, RX_ADDR_P0, val, 5);
		WriteToWireless(W, TX_ADDR, val, 5);
		_delay_ms(100);
}




int main(void)
{
	int players;
	int player1score = 0;
	int player2score = 0;
	int maxturns;
	maxturns = 15;
	int turn;
	int playerloop;
	players = 1;
	int j;
	int loop = 1;
	lcd_init();
	init();
	initSPI();
	initWireless();
	USART_SPI_Init();
	lcd_string("Waiting");
	
	while((GetReg(STATUS) & (1<< 6)) == 0)
	receieve_payload();	
	data = WriteToWireless(R, R_RX_PAYLOAD, data, 5);
	
	resetWireless();
	
	if(data[0] == 2) {
		players = 2;
	}
	lcd_command(0x01);
	lcd_string("# of Players: ");
	if(players == 2) {
		lcd_char('2');
	}
	else {
		lcd_char('1');
	}
	for(turn = 0; turn < maxturns; turn++) {
		for(j = 0; j < players; j++) {
			_delay_ms(1000);
			changeToTransmit();
			changeAddress(j);
			data[0] = 0x01;
			data[1] = 0x01;
			data[2] = 0x01;
			data[3] = 0x01;
			data[4] = 0x01;
			transmit_payload(data);
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
			changeToReceive();
			playerloop = 1;
			lcd_command(0x01);
			lcd_string("Player ");
			if(j == 0) {
				lcd_char('1');
			}
			else {
				lcd_char('2');
			}
			lcd_string(" Turn");
			lcd_command(0xC0);
			lcd_num(player1score);
			if(players == 2) {
				lcd_string("        ");
				lcd_num(player2score);
			}
			while(playerloop) {
				lcd_command(0x01);
				lcd_string("Player ");
				if(j == 0) {
					lcd_char('1');
				}
				else {
					lcd_char('2');
				}
				lcd_string(" Turn");
				lcd_command(0xC0);
				lcd_num(player1score);
				if(players == 2) {
					lcd_string("        ");
					lcd_num(player2score);
				}
				while((GetReg(STATUS) & (1<< 6)) == 0) {
					receieve_payload();
				}
				data = WriteToWireless(R, R_RX_PAYLOAD, data, 5);
				resetWireless();
				if(data[0] == 1) {
					int number;
					number = getDice();
					data[0] = (uint8_t)number;
					data[1] = (uint8_t)number;
					data[2] = (uint8_t)number;
					data[3] = (uint8_t)number;
					data[4] = (uint8_t)number;
					changeToTransmit();
					lcd_command(0x01);
					lcd_string("Transmitting: ");
					if(data[0] == 1) {
						lcd_char('1');
					}
					else if(data[0] == 2) {
						lcd_char('2');
					}
					else if(data[0] == 3) {
						lcd_char('3');
					}
					else if(data[0] == 4) {
						lcd_char('4');
					}
					else if(data[0] == 5) {
						lcd_char('5');
					}
					else {
						lcd_char('6');
					}
					_delay_ms(1000);
					transmit_payload(data);
					loop = 1;
					while(loop) {
						if((GetReg(STATUS) & (1<< 5)) != 0) {
							loop = 0;
						}
						if((GetReg(STATUS) & (1<<4)) != 0) {
							resetWireless();
							transmit_payload(data);
						}
					}
					resetWireless();
					changeToReceive();
				}
				else {
					playerloop = 0;
					if(j == 0) {
						player1score = 0;
						player1score = data[1];
						player1score <<= 8;
						player1score |= data[2];
					}
					else {
						player2score = 0;
						player2score = data[1];
						player2score <<= 8;
						player2score |= data[2];
					}
					lcd_command(0x01);
					lcd_string("Ending turn");
				}
			}	
		}	
	}
	lcd_command(0x01);
	lcd_string("End of game");
	_delay_ms(1000);
	resetWireless();
	changeAddress(0);
	changeToTransmit();
	if(players == 1) {
		data[0] = 0x01;
		transmit_payload(data);
		loop = 1;
		while(loop) {
			if((GetReg(STATUS) & (1<< 5)) != 0) {
				loop = 0;
			}
			if((GetReg(STATUS) & (1<<4)) != 0) {
				resetWireless();
				transmit_payload(data);
			}
		}
		resetWireless();
	}
	else {
		if(player1score > player2score) {
			data[0] = 0x01;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
			changeAddress(1);
			_delay_ms(1000);
			data[0] = 0x00;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
		}
		else if(player2score > player1score) {
			data[0] = 0x00;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
			changeAddress(1);
			_delay_ms(1000);
			data[0] = 0x01;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
		}
		else {
			data[0] = 0x02;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
			changeAddress(1);
			_delay_ms(1000);
			data[0] = 0x02;
			transmit_payload(data);
			loop = 1;
			while(loop) {
				if((GetReg(STATUS) & (1<< 5)) != 0) {
					loop = 0;
				}
				if((GetReg(STATUS) & (1<<4)) != 0) {
					resetWireless();
					transmit_payload(data);
				}
			}
			resetWireless();
		}
	}
	while(1);
	
}
