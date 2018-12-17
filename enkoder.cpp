/*
 * enkoder.cpp
 *
 *  Created on: 17-11-2017
 *      Author: theoar
 */


#include "enkoder.h"
#include "pin.h"

void enkoder_setupt(Enkoder * Enc,
					uint8_t LEFT_EVENT,
					uint8_t RIGHT_EVENT,
					uint8_t Type)
{
	Enc->EventLeft = LEFT_EVENT;
	Enc->EventRight = RIGHT_EVENT;
	Enc->Type = Type;
}

void enkoder_setupt_pin(Enkoder * Enc,
		volatile uint8_t * DirReg,
		volatile uint8_t * InReg,
		volatile uint8_t * OutReg,
		uint8_t  Pin,
		uint8_t WhichPin)
{
	pin_setup(&(Enc->Pins[WhichPin]), DirReg, InReg, OutReg, Pin);
	pin_as_input(&(Enc->Pins[WhichPin]));
	pin_enable_pullup(&(Enc->Pins[WhichPin]));
}

uint8_t enkoder_read_mask(Enkoder * Enc)
{
	Enc->Mask <<= 2;
	Enc->Mask &= 15;
	Enc->Mask |= (pin_read_bit(&Enc->Pins[0]) << 1) | (pin_read_bit(&Enc->Pins[1]) << 0);

	if (Enc->Mask==14  || ((Enc->Type==Full) & (Enc->Mask==1 || Enc->Mask==8 || Enc->Mask==7)))
	{
		return 1 << Enc->EventLeft;
	}
	else if(Enc->Mask==13 || ((Enc->Type==Full) & (Enc->Mask==2 || Enc->Mask==11 || Enc->Mask==4)))
	{
		return 1 << Enc->EventRight;
	}

	return 0;
}



