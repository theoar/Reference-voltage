/*
 * key.cpp
 *
 *  Created on: 17-11-2017
 *      Author: theoar
 */
#include <stdio.h>

#include "key.h"
#include "hd44780.h"

void key_setup(Key * KeyObj, uint8_t PUSH_EVENT, uint8_t PULL_EVENT,
		uint8_t HOLD_EVENT, uint8_t ACTIVE_STATE, uint16_t TIME)
{
	KeyObj->PushEvent = PUSH_EVENT;
	KeyObj->PullEvent = PULL_EVENT;
	KeyObj->HoldEvent = HOLD_EVENT;
	KeyObj->ActiveState = ACTIVE_STATE;
	KeyObj->Time = TIME;
	KeyObj->CurrentTime = 0;
	KeyObj->Block = 0;
}

void key_setup_pin(Key * KeyObj,
		volatile uint8_t * DirReg,
		volatile uint8_t * InReg,
		volatile uint8_t * OutReg,
		uint8_t Pin)
{
	pin_setup(&KeyObj->PinSettings, DirReg, InReg, OutReg, Pin);
	pin_as_input(&KeyObj->PinSettings);
	pin_enable_pullup(&KeyObj->PinSettings);
}


uint8_t key_read_mask(Key * KeyObj)
{
	if(KeyObj->CurrentTime!=0)
		KeyObj->CurrentTime--;

	uint8_t State = pin_read_bit(&KeyObj->PinSettings);
	if(State == KeyObj->ActiveState  && KeyObj->Block==0)
	{
		KeyObj->CurrentTime = KeyObj->Time;
		KeyObj->Block = 1;
		return 1 << KeyObj->PushEvent;
	}
	else
	{
		if(State == KeyObj->ActiveState &&  KeyObj->CurrentTime==0)
		{
			KeyObj->CurrentTime = KeyObj->Time;
			return 1 << KeyObj->HoldEvent;
		}
		else if(State != KeyObj->ActiveState && KeyObj->Block==1)
		{

			KeyObj->Block = 0;
			KeyObj->CurrentTime = 0;
			return 1<<KeyObj->PullEvent;
		}
	}

	return 0;
}
