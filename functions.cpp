/*
 * functions.cpp
 *
 *  Created on: 21-11-2017
 *      Author: theoar
 */

#include  "functions.h"


uint8_t str_len(const char *What)
{
	uint8_t X = 0;
	while(What[X]!='\0')
		X++;

	return X;
}

void str_cpy(char *Where, const char *What)
{
	uint8_t X = 0;
	while(What[X]!='\0')
	{
		Where[X] = What[X];
		X++;
	}
	Where[X] = '\0';
}
