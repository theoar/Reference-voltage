/*
 * enkoder.h
 *
 *  Created on: 17-11-2017
 *      Author: theoar
 */

#ifndef ENKODER_H_
#define ENKODER_H_

#include "pin.h"

enum EnkoderType{
	Half = 0,
	Full = 1
};

enum EnkoderDirection
{
	Right = 0,
	Left = 1
};

typedef struct ENKODER{
	Pin Pins[2];
	uint8_t Type;
	uint8_t EventLeft;
	uint8_t EventRight;
	uint8_t Mask;
} Enkoder;


void enkoder_setupt(Enkoder * Enc,
					uint8_t LEFT_EVENT,
					uint8_t RIGHT_EVENT,
					uint8_t Type);

void enkoder_setupt_pin(Enkoder * Enc,
		volatile uint8_t * DirReg,
		volatile uint8_t * InReg,
		volatile uint8_t * OutReg,
		uint8_t  Pin,
		uint8_t WhihPin);

uint8_t enkoder_read_mask(Enkoder * Enc);



#endif /* ENKODER_H_ */
