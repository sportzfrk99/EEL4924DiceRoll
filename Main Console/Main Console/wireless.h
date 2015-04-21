/*
 * wireless.h
 *
 * Created: 4/13/2015 9:35:10 PM
 *  Author: sportzfrk
 */ 


#ifndef WIRELESS_H_
#define WIRELESS_H_
uint8_t GetReg(uint8_t reg);
uint8_t * WriteToWireless(uint8_t ReadWrite, uint8_t reg, uint8_t * val, uint8_t antVal);
void initWireless(void);
void transmit_payload(uint8_t * W_buff);
void receieve_payload(void);
void resetWireless(void);
void changeToReceive(void);
void changeToTransmit(void);





#endif /* WIRELESS_H_ */