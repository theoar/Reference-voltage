/*
 * key.h
 *
 *  Created on: 17-11-2017
 *      Author: theoar
 */

#ifndef KEY_H_
#define KEY_H_

#include "pin.h"

enum KeyActiveState{
	LOW = 0,
	HIGH = 1
};

typedef struct KEY
{
	Pin PinSettings;
	uint8_t PushEvent;
	uint8_t PullEvent;
	uint8_t HoldEvent;
	uint8_t ActiveState;
	uint8_t Block;

	uint16_t CurrentTime;
	uint16_t Time;

} Key;

void key_setup(Key * KeyObj, uint8_t PUSH_EVENT, uint8_t PULL_EVENT,
		uint8_t HOLD_EVENT, uint8_t ACTIVE_STATE, uint16_t TIME);

void key_setup_pin(Key * KeyObj,
		volatile uint8_t * DirReg,
		volatile uint8_t * InReg,
		volatile uint8_t * OutReg,
		uint8_t Pin);

uint8_t key_read_mask(Key * KeyObj);



#endif /* KEY_H_ */
