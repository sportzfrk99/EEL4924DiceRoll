/*
 * wireless.c
 *
 * Created: 4/13/2015 9:24:05 PM
 *  Author: sportzfrk
 */ 
#include <avr/io.h>
#include <inttypes.h>
#include "nordic.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include "SPI.h"


uint8_t GetReg(uint8_t reg) {
	_delay_us(10);
	PORTB &= ~(1<<PB2); // SS Wireless Low
	_delay_us(10);
	SPI_RW(R_REGISTER + reg); // Send read register command
	_delay_us(10);
	reg = SPI_RW(NOP); // Send a NOP to get data back (1 NOP per byte)
	_delay_us(10);
	PORTB |= (1<<PB2); // SS Wireless High
	return reg;
}

uint8_t * WriteToWireless(uint8_t ReadWrite, uint8_t reg, uint8_t * val, uint8_t antVal) {
	if(ReadWrite == W) {
		reg = W_REGISTER + reg;
	}
	
	static uint8_t ret[32];
	
	_delay_us(10);
	PORTB &= ~(1<<PB2);
	_delay_us(10);
	SPI_RW(reg);
	_delay_us(10);
	
	int i;
	for(i=0; i<antVal; i++) {
		if(ReadWrite == R && reg != W_TX_PAYLOAD) {
			ret[i]=SPI_RW(NOP);
			_delay_us(10);
		}
		else {
			SPI_RW(val[i]);
			_delay_us(10);
		}
	}
	PORTB |= (1<<PB2);
	
	return ret;
}

void initWireless(void) {
	_delay_ms(100);
	
	uint8_t val[5];
	
	val[0] = 0x01;
	WriteToWireless(W, EN_AA, val, 1);
	
	val[0] = 0x2F;
	WriteToWireless(W, SETUP_RETR, val, 1);
	val[0] = 0x01;
	WriteToWireless(W, EN_RXADDR, val, 1);
	val[0] = 0x03;
	WriteToWireless(W, SETUP_AW, val, 1);
	
	val[0] = 0x01;
	WriteToWireless(W, RF_CH, val, 1);
	
	val[0] = 0x27;
	WriteToWireless(W, RF_SETUP, val, 1);
	
	int i;
	for(i=0; i<5;i++) {
		val[i] = 0x12;
		
	}
	WriteToWireless(W, RX_ADDR_P0, val, 5);
	WriteToWireless(W, TX_ADDR, val, 5);
	val[0] = 5;
	WriteToWireless(W, RX_PW_P0, val, 1);
	val[0] = 0x1F;
	WriteToWireless(W, CONFIG, val, 1);
	
	_delay_ms(100);
	
	
}

void changeToTransmit(void) {
	_delay_ms(100);
	uint8_t val[5];
	val[0] = 0x1E;
	WriteToWireless(W, CONFIG, val, 1);
	_delay_ms(100);
}

void changeToReceive(void) {
	_delay_ms(100);
	uint8_t val[5];
	val[0] = 0x1F;
	WriteToWireless(W, CONFIG, val, 1);
	_delay_ms(100);
}

void transmit_payload(uint8_t * W_buff) {
	WriteToWireless(R, FLUSH_TX, W_buff, 0);
	WriteToWireless(R, W_TX_PAYLOAD, W_buff, 5);
	
	_delay_ms(10);
	PORTB |= (1<<PB1);
	_delay_us(20);
	PORTB &= ~(1<<PB1);
	_delay_ms(10);
}

void receieve_payload(void) {
	PORTB |= (1<<PB1);
	_delay_ms(1000);
	PORTB &= ~(1<<PB1);
}

void resetWireless(void) {
	_delay_us(10);
	PORTB &= ~(1<<PB2);
	_delay_us(10);
	SPI_RW(W_REGISTER + STATUS);
	_delay_us(10);
	SPI_RW(0x70);
	_delay_us(10);
	PORTB |= (1<<PB2);
}

