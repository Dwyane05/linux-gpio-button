/*
 * gpio.cpp
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
#include "gpio.h"

#define DBG_EN 	0
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[gpio_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[gpio]INFO: "x,##arg)
#define printf_warn(x,arg...) printf("[gpio]WARNING: "x,##arg)
#define printf_err(x,arg...) printf("[gpio]ERROR: "x,##arg)

static int sunxi_name_to_gpio(char const *name, char *gpio)
{
	char bank;
	int num;

	if (!name || !gpio) {
		return -EINVAL;
	}

	sscanf(name,"P%c%d", &bank, &num);
	if(num >= 32 || num < 0) {
		return -EINVAL;
	}

	if (bank >= 'a' && bank <= 'z') {
		num = num + SUNXI_BANK_SIZE * (bank - 'a');
	} else if (bank >= 'A' && bank <= 'Z') {
		num = num + SUNXI_BANK_SIZE * (bank - 'A');
	} else {
		return -EINVAL;
	}

	return sprintf(gpio, "%d",  num);
}

int init_gpio(struct gpio *gpio, char const *pin_name)
{
	int fd_gpio;
	int ret;

	if(!gpio || !pin_name) {
		return -EINVAL;
	}
	if(gpio->gpio_num[0] != '\0') {
		return -EINVAL;
	}

	ret = sunxi_name_to_gpio(pin_name, gpio->gpio_num);
	if(ret < 0) {
		printf_err("get name fail,%d\n",ret);
		return -ret;
	}
	gpio->direction = GPIO_INVAL;

	//设置GPIO管脚
	fd_gpio = open("/sys/class/gpio/export",O_WRONLY);
	if(fd_gpio < 0)
	{
		printf_err("Export gpio fail,%d\n",ret);
		return fd_gpio;
	}

	printf_dbg("init gpio_num:%s\n",gpio->gpio_num);

	ret = write(fd_gpio, gpio->gpio_num, NUM_MAXLEN);
	if(ret < 0) {
		//printf_err("gpio write init error.(%d)\n",ret);
		int fd_gpio_exit = open("/sys/class/gpio/unexport", O_WRONLY);
		if(fd_gpio_exit == -1)
		{
			printf_err("Open and try unexport gpio error\n");
			return -1;
		} else {
			printf_dbg("try unexport gpio_num:%s\n",gpio->gpio_num);

			ret = write(fd_gpio_exit, gpio->gpio_num, NUM_MAXLEN);
			if(ret < 0) {
				printf_err("ERR: gpio write unexport error.\n");
			}
			close(fd_gpio_exit);
		}

		ret = write(fd_gpio, gpio->gpio_num, NUM_MAXLEN);
		if(ret < 0) {
			printf_err("Try again gpio write init error.\n");
			return ret;
		}
	}

	close(fd_gpio);

	char gpio_path[50] = "/sys/class/gpio/gpio";

	strcpy(gpio->gpio_direction,gpio_path);
	strcat(gpio->gpio_direction,gpio->gpio_num);
	strcat(gpio->gpio_direction,"/direction");
	printf_dbg("gpio direction path is : %s\n", gpio->gpio_direction);

	strcpy(gpio->gpio_value,gpio_path);
	strcat(gpio->gpio_value,gpio->gpio_num);
	strcat(gpio->gpio_value,"/value");
	printf_dbg("gpio value path is : %s\n", gpio->gpio_value);

	return 0;
}

int exit_gpio(struct gpio *gpio)
{
	int fd_gpio;
	int ret = 0;

	if(gpio->gpio_num[0] == '\0') {
		return -EINVAL;
	}

	fd_gpio = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd_gpio == -1)
	{
		printf_err("open unexport file fail.\n");
	}

	printf_dbg("exit gpio_num:%s\n",gpio->gpio_num);

	ret = write(fd_gpio, gpio->gpio_num, NUM_MAXLEN);
	if(ret < 0) {
		printf_err("write exit error.\n");
	}
	close(fd_gpio);

	gpio->direction = GPIO_INVAL;
	gpio->gpio_num[0] = '\0';
	return ret;
}

int gpio_set_direction(struct gpio *gpio, int flags)
{
	int fd_gpio;
	int ret = 0;

	fd_gpio = open(gpio->gpio_direction, O_WRONLY);
	if(fd_gpio == -1)
	{
		printf_err("open %s fail\n",gpio->gpio_direction);
		return -EIO;
	}

	//设置GPIO方向
	if(GPIO_OUT == flags) {
		ret = write(fd_gpio,"out",sizeof("out"));
		if(ret < 0) {
			goto direction_out;
		}
		gpio->direction = GPIO_OUT;
	} else if(GPIO_IN == flags) {
		ret= write(fd_gpio,"in",sizeof("in"));
		if(ret < 0) {
			goto direction_out;
		}
		gpio->direction = GPIO_IN;
	} else {
		gpio->direction = GPIO_INVAL;
		return -EINVAL;
	}

direction_out:
	if(ret < 0) {
		printf_err("ERR: gpio direction write error.\n");
	}

	close(fd_gpio);
	return ret;
}

int gpio_output_open(struct gpio *gpio)
{
	int fd_gpio;

	if(gpio->direction != GPIO_OUT) {
		return -EIO;
	}

	fd_gpio = open(gpio->gpio_value, O_WRONLY);
	if(fd_gpio == -1)
	{
		printf_err("open %s\n",gpio->gpio_value);
		return -EIO;
	}

	return fd_gpio;
}

int gpio_output_close(struct gpio *gpio, int fd_gpio)
{
	if(!gpio || (fd_gpio < 0)) {
		return -EINVAL;
	}
	if(gpio->direction != GPIO_OUT) {
		return -EIO;
	}

	close(fd_gpio);
	return 0;
}

