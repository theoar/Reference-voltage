/*
 * io_events.cpp
 *
 *  Created on: 13-11-2017
 *      Author: theoar
 */
#include "io_events.h"
#include "pin.h"
#include "enkoder.h"
#include "key.h"
#include "hd44780.h"


#include <avr/interrupt.h>


Events EventsObject;
void io_events_init(void)
{
	TCCR0A |= 2;
	OCR0A = 124;
	TIMSK0 |= 2;
	TCCR0B |= 4;

}

uint16_t io_events_read(void)
{
	TIMSK0 = 0;
	uint16_t X = EventsObject.Event;
	EventsObject.Event = 0;
	TIMSK0 |= 2;

	return X;
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
	TIMSK0 = 0;

	for(uint8_t X = 0; X<ENCODERS_COUNT; ++X)
		EventsObject.Event |= enkoder_read_mask(&EventsObject.EncTab[X]);
	for(uint8_t X = 0; X<BUTTONS_COUNT; ++X)
		EventsObject.Event |= key_read_mask(&EventsObject.KeyTab[X]);


	TIMSK0 |= 2;
}

void io_events_add_enc(Enkoder *Enc)
{
	EventsObject.EncTab[EventsObject.EncIndex]  = *Enc;
	(EventsObject.EncIndex)++;
}

void io_events_add_key(Key *KeyObj)
{
	EventsObject.KeyTab[EventsObject.KeyIndex] = *KeyObj;
	(EventsObject.KeyIndex)++;
}

void io_events_disable_propagation(void)
{
	TIMSK0 = 0;
	EventsObject.Event = 0;
}

void io_events_enable_propagation(void)
{
	TIMSK0 |= 2;
}

void io_events_add_my_io(void)
{
	Enkoder Enc1;
	Enkoder Enc2;
	enkoder_setupt(&Enc1, ENKODERA_LEFT, ENKODERA_RIGHT, Half);
	enkoder_setupt(&Enc2, ENKODERB_LEFT, ENKODERB_RIGHT, Half);

	enkoder_setupt_pin(&Enc1, &DDRC, &PINC, &PORTC, PINC5, 0);
	enkoder_setupt_pin(&Enc1, &DDRC, &PINC, &PORTC, PIND4, 1);

	enkoder_setupt_pin(&Enc2, &DDRC, &PINC, &PORTC, PINC3, 0);
	enkoder_setupt_pin(&Enc2, &DDRC, &PINC, &PORTC, PINC2, 1);

	io_events_add_enc(&Enc1);
	io_events_add_enc(&Enc2);

	Key Key1;
	Key Key2;

	key_setup_pin(&Key1, &DDRC, &PINC, &PORTC, PINC0);
	key_setup_pin(&Key2, &DDRC, &PINC, &PORTC, PINC1);

	key_setup(&Key1, KEYA_PUSH, KEYA_PULL, KEYA_HOLD, LOW, 1000);
	key_setup(&Key2, KEYB_PUSH, KEYB_PULL, KEYB_HOLD, LOW, 1000);

	io_events_add_key(&Key1);
	io_events_add_key(&Key2);

}
