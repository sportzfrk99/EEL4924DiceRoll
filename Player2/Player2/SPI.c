/*
 * SPI.c
 *
 * Created: 4/13/2015 9:20:13 PM
 *  Author: sportzfrk
 */ 

#include <avr/io.h>

void initSPI(void) {
	
	// Enable Port B bits as outputs where necessary
	DDRB |= (1<<DDB7) | (1<<DDB5) | (1<<DDB4) | (1<<DDB2) | (1<<DDB1);
	
	// Sets up the SPCR register for SPI. Clock rate is fck/8
	SPSR |= (1<<SPI2X);
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	PORTB |= (1<<PB2) | (1<<PB0);
	PORTB &= ~(1<<PB1);
}

char SPI_RW(unsigned char cData) {
	SPDR = cData;
	
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}