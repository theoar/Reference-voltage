/*
 * io_events.h
 *
 *  Created on: 13-11-2017
 *      Author: theoar
 */

#define ENCODERS_COUNT 2
#define BUTTONS_COUNT 2

#define ENKODERA_LEFT 0
#define ENKODERA_RIGHT 1
#define ENKODERB_LEFT 2
#define ENKODERB_RIGHT 3

#define KEYA_PUSH 4
#define KEYA_PULL 5
#define KEYA_HOLD 6

#define KEYB_PUSH 7
#define KEYB_PULL 8
#define KEYB_HOLD 9

#ifndef IO_EVENTS_H_
#define IO_EVENTS_H_

#include <inttypes.h>

#include "enkoder.h"
#include "key.h"

typedef struct EVENTS
{
	volatile uint16_t Event;
	uint8_t EncIndex = 0;
	uint8_t KeyIndex = 0;
	Enkoder EncTab[ENCODERS_COUNT];
	Key KeyTab[BUTTONS_COUNT];

} Events;

void io_events_init(void);
void io_events_add_my_io(void);

uint16_t io_events_read(void);

void io_events_add_enc(Enkoder *Enc);
void io_events_add_key(Key *KeyObj);
void io_events_disable_propagation(void);
void io_events_enable_propagation(void);




#endif /* IO_EVENTS_H_ */
