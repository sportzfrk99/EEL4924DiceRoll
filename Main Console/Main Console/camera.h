/*
 * camera.h
 *
 * Created: 4/13/2015 11:45:42 PM
 *  Author: sportzfrk
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_
void initcamera();
int getStart(void);
uint16_t getBlocks(uint16_t maxBlocks);
int setServos(uint16_t s0, uint16_t s1);
int setBrightness(uint8_t brightness);
int setLED(uint8_t r, uint8_t g, uint8_t b);
extern uint8_t getByte(uint8_t out);



#endif /* CAMERA_H_ */