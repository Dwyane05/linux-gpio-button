/*
 * led.h
 *
 *  Created on: 5 Jul 2016
 *      Author: cui
 */

#ifndef LED_H_
#define LED_H_


#include "gpio.h"

class led {
private:
	int fd;

public:
	led();
	struct gpio led_gpio;
	int init_led(char const *pin_name);
	int exit_led();
	int on(void);
	int off(void);

private:
	void  delay(unsigned int i);
};




#endif /* LED_H_ */
