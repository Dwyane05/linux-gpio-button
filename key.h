/*
 * key.h
 *
 *  Created on: 5 Jul 2016
 *      Author: cui
 */

#ifndef KEY_H_
#define KEY_H_


#include <linux/input.h>

class key {
private:
	int fd;
	char name[10];

public:
	key(char const *key_name);
	key(void);
	int dev_open(char const *input_dev);
	int dev_open_nonblock(char const *input_dev);
	int dev_detect(char const *name);
	int dev_close();
	int get_val();
};

#endif /* KEY_H_ */
