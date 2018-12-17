/*
 * precisepwm.h
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#include <inttypes.h>

#include "float_t.h"
#include "timerone.h"

#ifndef PRECISEPWM_H_
#define PRECISEPWM_H_

enum UNIT
{
	HZ = 0,
	KHZ = 3,
	MHZ = 6,
	GHZ = 9,
};

typedef struct PRECISEPWM
{
	typedef struct CHANNELDATA{
		volatile uint16_t Duty;	//wartosc wypelnienia
		volatile uint8_t ExtraDutySuma; //obecna wartosc ekstra okresu
		volatile uint8_t CurrentExtraSkok; //obecna wartosc ekstra okresu
		uint8_t Mode = 0;
	} ChannelData;

	ChannelData ChannelA;
	ChannelData ChannelB;

	volatile uint8_t Diver; //stopien rozszerzenia precyzji
	uint16_t Period;
	Float_t VReference;
	Float_t VBasicStep;
	Float_t VPreciseStep;

	uint8_t Prescaller;

} PrecisePwm;

extern PrecisePwm PwmObject;

void pwm_init(void);
void pwm_start(void);
void pwm_stop(void);
Float_t pwm_setup(Float_t * VRef, Float_t * VBStep, Float_t * VPStep, uint8_t Prescaller);
void pwm_setup_channel(uint8_t Channel, uint8_t Mode);

Float_t pwm_set_channelA(Float_t Value);
Float_t pwm_set_channelB(Float_t Value);
Float_t pwm_set_channel(Float_t Value, uint8_t Channel);

Float_t pwm_get_curent(uint8_t Channel);

Float_t pwm_get_freq(uint8_t UNIT = KHZ);
Float_t pwm_get_basic_step(void);
Float_t pwm_get_precise_step(void);
Float_t pwm_get_reference(void);
Float_t pwm_get_period(void);

Float_t pwm_get_duty(uint8_t Channel);
Float_t pwm_get_extra_duty(uint8_t Channel);
Float_t pwm_get_extra_period(void);

void pwm_handle_interrupt_channelA(void);
void pwm_handle_interrupt_channelB(void);

bool pwm_is_running(void);



#endif /* PRECISEPWM_H_ */
