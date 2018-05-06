/*
 * buzzer.cpp
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
#include "buzzer.h"
#include "gpio.h"

#define DBG_EN 	0
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[buzzer_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[buzzer]INFO: "x,##arg)
#define printf_warn(x,arg...) printf("[buzzer]WARNING: "x,##arg)
#define printf_err(x,arg...) printf("[buzzer]ERROR: "x,##arg)


buzzer::buzzer()
{
	enabled = 1;
}

int buzzer::init_buzzer(char const *pin_name)
{
	int ret;

	memset(&buzzer_gpio, 0, sizeof(buzzer_gpio));

	printf_dbg("[buzzer] init pin : %s\n",pin_name);

	ret = init_gpio(&buzzer_gpio, pin_name);
	if(ret < 0) {
		printf_err("init gpio fail\n");
		return ret;
	}
	ret = gpio_set_direction(&buzzer_gpio, GPIO_OUT);
	if(ret < 0) {
		printf_err("set gpio direction fail\n");
		return ret;
	}
	return 0;
}

int buzzer::exit_buzzer()
{
	return exit_gpio(&buzzer_gpio);
}

void buzzer::enable()
{
	enabled = 1;
}

void buzzer::disable()
{
	enabled = 0;
}

void buzzer::delay(unsigned int i)
{
    unsigned char j;
    while(i--){
        for(j=0; j < 150; j++);
    }
}

int buzzer::buzzer_high(int longshort ,int times)
{
	int fd_gpio = -1;
	int i = 500;
	int k = times;
	if(!enabled) return -1;

	fd_gpio = gpio_output_open(&buzzer_gpio);

	for(; k >=0 ;k--)
	{
//		delay(20000);
		delay(longshort);
		for(i=0; i<500;i++)
		{
			write(fd_gpio,"1",sizeof("1"));
			delay(200);
			write(fd_gpio,"0",sizeof("0"));
			delay(200);
		}
	}
	gpio_output_close(&buzzer_gpio, fd_gpio);
	return 0;
}

int buzzer::buzzer_low(int longshort, int times)
{
	int fd_gpio = -1;
	int i = 100;
	int k = times;

	if(!enabled) return -1;

	fd_gpio = gpio_output_open(&buzzer_gpio);

	for(; k >=0 ;k--)
	{
//		delay(20000);
		delay(longshort);
		for(i=0; i<500;i++)
		{
			write(fd_gpio,"1",sizeof("1"));
			delay(2000);
			write(fd_gpio,"0",sizeof("0"));
			delay(2000);
		}
	}

	gpio_output_close(&buzzer_gpio, fd_gpio);
	return 0;
}


int buzzer::buzzer_openmusci()
{
	int fd_gpio = -1;
//	int i = 200;
//	int j = 1;

	if(!enabled) return -1;


	fd_gpio = gpio_output_open(&buzzer_gpio);

	for(int i=0; i<200; i++)
	{
		write(fd_gpio,"1",sizeof("1"));
		delay(240);
		write(fd_gpio,"0",sizeof("0"));
		delay(240);
	}

	for(int i=0; i<200; i++)
	{
		write(fd_gpio,"1",sizeof("1"));
		delay(200);
		write(fd_gpio,"0",sizeof("0"));
		delay(200);
	}


	for(int i=0; i<200; i++)
	{
		write(fd_gpio,"1",sizeof("1"));
		delay(240);
		write(fd_gpio,"0",sizeof("0"));
		delay(240);
	}

	for(int i=0; i<200; i++)
	{
		write(fd_gpio,"1",sizeof("1"));
		delay(200);
		write(fd_gpio,"0",sizeof("0"));
		delay(200);
	}

	gpio_output_close(&buzzer_gpio, fd_gpio);
	return 0;
}

//bu.buzzer_high(2);	//long
//bu.buzzer_high(1);	//short
//bu.buzzer_low(1);		//long
//bu.buzzer_low(0);		//short
int buzzer::buzzer_beep(unsigned char flag)
{
	switch(flag)
	{
	case ONESHORTHI:
		buzzer_high(2000, 0);
		break;

	case TWOSHORTHI:
		buzzer_high(2000, 1);
		break;

	case THREESHORTHI:
		buzzer_high(2000, 2);
		break;
	
	case FOURSHORTHI:
		buzzer_high(2000, 3);
		break;
	
	case FIVESHORTHI:
		buzzer_high(2000, 4);
		break;
	
	case ONESHORTLO:
		buzzer_low(2000, 0);
		break;
	
	case TWOSHORTLO:
		buzzer_low(2000, 1);
		break;
	
	case THREESHORTLO:
		buzzer_low(2000, 2);
		break;

	case FOURSHORTLO:
		buzzer_low(2000, 3);
		break;
	
	case FIVESHORTLO:
		buzzer_low(2000, 4);
		break;
	
	case ONELONGHI:
		buzzer_high(20000, 0);
		break;
	
	case TWOLONGHI:
		buzzer_high(20000, 1);
		break;
	
	case THREELONGHI:
		buzzer_high(20000, 2);
		break;
	
	case FOURLONGHI:
		buzzer_high(20000, 3);
		break;
	
	case FIVELONGHI:
		buzzer_high(20000, 4);
		break;
	
	case ONELONGLO:
		buzzer_low(20000, 0);
		break;
	
	case TWOLONGLO:
		buzzer_low(20000, 1);
		break;
	
	case THREELONGLO:
		buzzer_low(20000, 2);
		break;
	
	case FOURLONGLO:
		buzzer_low(20000, 3);
		break;
	
	case FIVELONGLO:
		buzzer_low(20000, 4);
		break;
	
	case FASTHILOHILO:
		buzzer_high(2000, 0);
		buzzer_low(2000, 0);
		buzzer_high(2000, 0);
		buzzer_low(2000, 0);
		break;
	
	case SLOWHILOHILO:
		buzzer_high(20000, 0);
		buzzer_low(20000, 0);
		buzzer_high(20000, 0);
		buzzer_low(20000, 0);
		break;
	
	case HILO:
		buzzer_high(2000, 0);
		buzzer_low(2000, 0);
		break;
	
	case LOHI:
		buzzer_low(2000, 0);
		buzzer_high(2000, 0);
		break;
	
	case HILOHI:
		buzzer_high(2000, 0);
		buzzer_low(2000, 0);
		buzzer_high(2000, 0);
		break;
	
	case LOHILO:
		buzzer_low(2000, 0);
		buzzer_high(2000, 0);
		buzzer_low(2000, 0);
		break;

	default:
		return -EINVAL;
		break;
	}
	
	return 0;
}
