/*
 * led.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: cui
 */

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "led.h"

#define DBG_EN 	0
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[led_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[gpio]INFO: "x,##arg)
#define printf_warn(x,arg...) printf("[gpio]WARNING: "x,##arg)
#define printf_err(x,arg...) printf("[gpio]ERROR: "x,##arg)

#define LED_ON_POLATIRY		1
#define LED_OFF_POLATIRY	0

led::led()
{
	fd = -1;
}

int led::init_led(char const *pin_name)
{
	int ret;

	memset(&led_gpio, 0, sizeof(led_gpio));

	printf_dbg("init pin : %s\n",pin_name);

	ret = init_gpio(&led_gpio, pin_name);
	if(ret < 0) {
		printf_err("init gpio fail,%d\n",ret);
		return ret;
	}
	ret = gpio_set_direction(&led_gpio, GPIO_OUT);
	if(ret < 0) {
		printf_err("set gpio output direction fail, %d\n",ret);
		return ret;
	}

	fd = gpio_output_open(&led_gpio);

	return fd;
}

int led::exit_led()
{
	gpio_output_close(&led_gpio, fd);
	return exit_gpio(&led_gpio);
}

int led::on(void)
{
#if (LED_ON_POLATIRY == 1)
	return write(fd, "1", sizeof("1"));
#elif (LED_ON_POLATIRY == 0)
	return write(fd, "0", sizeof("0"));
#else
#error Unknow LED POLATIRY
#endif
}

int led::off(void)
{
#if (LED_OFF_POLATIRY == 1)
	return write(fd, "1", sizeof("1"));
#elif (LED_OFF_POLATIRY == 0)
	return write(fd, "0", sizeof("0"));
#else
#error Unknow LED POLATIRY
#endif
}
