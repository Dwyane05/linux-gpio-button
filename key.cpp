/*
 * key.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: cui
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "key.h"

#define DBG_EN 0
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[key_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[key] INFO:"x,##arg)
#define printf_warn(x,arg...) printf("[key] WARNING"x,##arg)
#define printf_err(x,arg...) printf("[key] ERROR:"x,##arg)

key::key(void)
{
	fd = -1;
}

key::key(char const *key_name)
{
	fd = -1;
	strcpy(name, key_name);
}

int key::dev_detect(char const *name)
{
	int i;
	char name_path[50];
	char key_name[20];
	int fd;

	printf_dbg("detect for %s\n", name);

	for(i = 0; i < 10; i++)
	{
		sprintf(name_path, "/sys/class/input/input%d/name",i);
		fd = open(name_path, O_RDONLY);
		if(fd < 0) {
			printf_err("%s open fail,(%d)\n",name_path,fd);
			close(fd);
			continue;
		}
		memset(key_name, 0, sizeof(key_name));
		int ret = read(fd, key_name, 20);
		if(ret < 0 ) {
			printf_err("%s read fail\n",key_name);
			close(fd);
			continue;
		}
		key_name[strlen(key_name) - 1] = '\0';

		printf_dbg("Get input%d name: %s\n", i, key_name);
		ret = strncmp(name, key_name, strlen(key_name));
		if(0 == ret) {
			close(fd);
			return i;
		}
		close(fd);
	}

	return -1;
}


int key::dev_open_nonblock(char const *input_dev)
{
	if(input_dev) {
		fd = open(input_dev, O_RDONLY | O_NONBLOCK);
		if (-1 == fd) {
		       perror(input_dev);
		       return -1;
		 }
	} else {
		int ret;
		//ret = dev_detect("sunxi-keyboard");
		ret = dev_detect("axp22-supplyer");
		if(ret < 0) {
			printf_err("detect device fail\n");
			return ret;
		}

		char event[20];
		sprintf(event, "/dev/input/event%d",ret);
		fd = open(event, O_RDONLY | O_NONBLOCK);
		if (-1 == fd) {
			perror(input_dev);
			return -1;
		}
	}

	return fd;
}

int key::dev_open(char const *input_dev)
{
	if(input_dev) {
		fd = open(input_dev, O_RDONLY);
		if (-1 == fd) {
			perror(input_dev);
			return -1;
		}
		printf_dbg("Input event device[%s] opened[%d]\n", input_dev, fd);
	} else {
		int ret;
		ret = dev_detect(name);
		if(ret < 0) {
			printf_err("detect device fail\n");
			return ret;
		}

		char event[20];
		sprintf(event, "/dev/input/event%d",ret);
		fd = open(event, O_RDONLY);
		if (-1 == fd) {
			perror(input_dev);
			return -1;
		}
		printf_dbg("Input event device[%s] opened[%d]\n", event, fd);
	}

	return fd;
}

int key::dev_close()
{
	if(fd < 0)
		return -1;

	close(fd);
	return 0;
}

int key::get_val()
{
	struct input_event ev;
	int flag = 1;
	int ret = -1;
	int trytimes = 3;

	while(flag)
	{
		if (read(fd, &ev, sizeof(struct input_event)) == sizeof(struct input_event))
		{
			switch (ev.type)
			{
				case EV_SYN:
					printf_dbg("keys %s\n", ret == 1?"down":(ret == 0?"up":"unregnize"));
					if(ret == 0)
						flag = 0;
					break;

		        case EV_KEY:
		        	if(ev.code == KEY_POWER)
		        		ret  = !!(ev.value);
		        	else
		        		printf_err("unknow key code\n");
		        	break;
			}
		} else {
			printf("read key val incorrect!\n");
			trytimes--;
			if(trytimes <= 0)
				flag = 0;
		}
	}

	return ret;
}
