/*
 * pin.h
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#ifndef PIN_H_
#define PIN_H_

#include <inttypes.h>

enum PinTypeEnum{
	INPUT = 0,
	OUTPUT =1
};

typedef struct PIN{
	volatile uint8_t * DirectionRegister;
	volatile uint8_t * InputRegister;
	volatile uint8_t * OutputRegister;
	uint8_t PinNumber;
} Pin;

void pin_setup(Pin *PtrPin, volatile uint8_t* DReg, volatile uint8_t* IReg, volatile uint8_t* OReg, uint8_t PNum);
void pin_as_output(Pin * PtrPin);
void pin_as_input(Pin * PtrPin);
void pin_write_bit(Pin * PtrPin, uint8_t Value);
uint8_t pin_read_bit(Pin * PtrPin);

#define pin_enable_pullup(PtrPin) pin_write_bit(PtrPin, 1)
#define pin_disable_pullup(PtrPin) pin_write_bit(PtrPin, 0)

void pin_toggle(Pin * PtrPin);



#endif /* PIN_H_ */
