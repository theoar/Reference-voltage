/*
 * pins.cpp
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */
#include <stdio.h>


#include "pin.h"

void pin_setup(Pin *PtrPin, volatile uint8_t* DReg, volatile uint8_t* IReg, volatile uint8_t* OReg, uint8_t PNum)
{
	PtrPin->DirectionRegister=DReg;
	PtrPin->InputRegister=IReg;
	PtrPin->OutputRegister=OReg;
	PtrPin->PinNumber=PNum;
}

void pin_as_output(Pin * PtrPin)
{
	*PtrPin->DirectionRegister |= (1<<(PtrPin->PinNumber));
}

void pin_as_input(Pin * PtrPin)
{
	*PtrPin->DirectionRegister &= ~(1<<(PtrPin->PinNumber));
}


void pin_write_bit(Pin * PtrPin, uint8_t Value)
{
	uint8_t X = ((*PtrPin->OutputRegister) & ~(1<<PtrPin->PinNumber)) | ((Value & 0x1) << PtrPin->PinNumber);
	*PtrPin->OutputRegister = X;
}

uint8_t pin_read_bit(Pin * PtrPin)
{
	return ((*PtrPin->InputRegister) >> PtrPin->PinNumber) & (0x1);
}


void pin_toggle(Pin * PtrPin)
{
	*PtrPin->OutputRegister ^= (1<<PtrPin->PinNumber);
}



