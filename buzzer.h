/*
 * buzzer.h
 *
 *  Created on: 4 Jul 2016
 *      Author: cui
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

#define ONESHORTHI 		0x00
#define TWOSHORTHI 		0x01
#define THREESHORTHI 	0x02
#define FOURSHORTHI 	0x03
#define FIVESHORTHI 	0x04
#define ONESHORTLO 		0x05
#define TWOSHORTLO 		0x06
#define THREESHORTLO 	0x07
#define FOURSHORTLO 	0x08
#define FIVESHORTLO 	0x09
#define ONELONGHI 		0x0A
#define TWOLONGHI 		0x0B
#define THREELONGHI		0x0C
#define FOURLONGHI 		0x0D
#define FIVELONGHI 		0x0E
#define ONELONGLO 		0x0F
#define TWOLONGLO 		0x10
#define THREELONGLO 	0x11
#define FOURLONGLO 		0x12
#define FIVELONGLO 		0x13
#define FASTHILOHILO 	0x14
#define SLOWHILOHILO 	0x15
#define HILO 			0x16
#define LOHI 			0x17
#define HILOHI 			0x18
#define LOHILO 			0x19

class buzzer {
private:
	int enabled;
	struct gpio buzzer_gpio;

	void  delay(unsigned int i);

public:
	buzzer();
	void enable();
	void disable();

	int init_buzzer(char const *pin_name);
	int exit_buzzer();
	int buzzer_high(int longshort, int times);
	int buzzer_low(int longshort, int times);

	int buzzer_openmusci();
	int buzzer_beep(unsigned char flag);

};




#endif /* BUZZER_H_ */
