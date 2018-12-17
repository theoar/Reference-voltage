/*
 * precisepwm.c
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#include "precisepwm.h"
#include "timerone.h"
#include "pin.h"
#include "float.h"

#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

PrecisePwm PwmObject;

ISR(TIMER1_OVF_vect)
{
	PwmObject.ChannelA.ExtraDutySuma += PwmObject.ChannelA.CurrentExtraSkok;
	OCR1A = PwmObject.ChannelA.Duty + (PwmObject.ChannelA.ExtraDutySuma / PwmObject.Diver);
	PwmObject.ChannelA.ExtraDutySuma %= PwmObject.Diver;
}


void pwm_init(void)
{
	t1_configure_mode(TimerOneFastPwmICR1);
	t1_enable_interrupts(OverflowInterrupt);
}

void pwm_start(void)
{
	t1_start(PwmObject.Prescaller);
}

void pwm_stop(void)
{
	t1_stop();
}

Float_t pwm_setup(Float_t * VRef, Float_t * VBStep, Float_t * VPStep, uint8_t Prescaller)
{
	t1_disable_interrupts(OverflowInterrupt);

	PwmObject.Prescaller = Prescaller;

	uint8_t PwmState = TCCR1B & 7;
	TCCR1B &=~(7);

	PwmObject.VReference = *VRef;

	Float_t Temp;
	if(!f_is_zero(VPStep))
	{
		Temp = *VBStep;
		fdiv(&Temp, VPStep);
		PwmObject.Diver=f_get_integer(&Temp);
	}
	else
		PwmObject.Diver = 0;

	Temp = *VRef;
	fdiv(&Temp,VBStep);
	PwmObject.Period = f_get_integer(&Temp);
	PwmObject.Period -= 1;

	Temp = *VRef;
	fdiv(&Temp,PwmObject.Period+1);
	PwmObject.VBasicStep = Temp;

	fdiv(&Temp, PwmObject.Diver);
	PwmObject.VPreciseStep = Temp;

	PwmObject.ChannelA.ExtraDutySuma = 0;
	PwmObject.ChannelA.CurrentExtraSkok = 0;
	PwmObject.ChannelA.Duty = ((PwmObject.Period+1)/2)-1;

	//PwmObject.ChannelB.ExtraDutySuma = 0;
	//PwmObject.ChannelB.CurrentExtraSkok = 0;
	//PwmObject.ChannelB.Duty = ((PwmObject.Period+1)/2)-1;

	ICR1 = PwmObject.Period;
	OCR1A = PwmObject.ChannelA.Duty;
	//OCR1B = PwmObject.ChannelB.Duty;

	TCCR1B |= PwmState;

	t1_enable_interrupts(OverflowInterrupt);

	return pwm_get_curent(TimerOneChannelA);
}

Float_t pwm_set_channel(Float_t Value, uint8_t Channel)
{
	t1_disable_interrupts(OverflowInterrupt);

	if(f_is_zero(&Value))
	{
		t1_configure_channel_mode(DisconnectChannel, Channel);
		Pin PinObject;
		pin_setup(&PinObject, &DDRB, &PINB, &PORTB, Channel == TimerOneChannelA ? PINB1 : PINB2);
		pin_write_bit(&PinObject, 0);
	}
	else if(t1_get_channel_mode(Channel)==DisconnectChannel)
	{
			switch(Channel)
			{
				case TimerOneChannelA:
					t1_configure_channel_mode(PwmObject.ChannelA.Mode, Channel);
					break;
				case TimerOneChannelB:
					t1_configure_channel_mode(PwmObject.ChannelB.Mode, Channel);
					break;
			}
	}

	PrecisePwm::ChannelData *Chan = (Channel==TimerOneChannelA) ? &PwmObject.ChannelA : &PwmObject.ChannelB;

	Float_t Temp = Value;
	fdiv(&Temp, &PwmObject.VBasicStep);
	Chan->Duty = f_get_integer(&Temp) - 1;


	Temp =  fmultiple_c(&PwmObject.VBasicStep, Chan->Duty+1);
	Temp = fminus_c(&Value, &Temp);
	fdiv(&Temp, &PwmObject.VPreciseStep);
	Chan->CurrentExtraSkok = f_get_integer(&Temp);

	Chan->ExtraDutySuma = 0;

	t1_enable_interrupts(OverflowInterrupt);

	return pwm_get_curent(Channel);
}

Float_t pwm_set_channelA(Float_t Value)
{
	return pwm_set_channel(Value, TimerOneChannelA);
}

Float_t pwm_set_channelB(Float_t Value)
{
	return pwm_set_channel(Value, TimerOneChannelB);
}

Float_t pwm_get_curent(uint8_t Channel)
{
	PrecisePwm::ChannelData *Chan = (Channel==TimerOneChannelA) ? &PwmObject.ChannelA : &PwmObject.ChannelB;

	Float_t CurrentValueBasic = PwmObject.VBasicStep;
	fmultiple(&CurrentValueBasic, Chan->Duty+1);

	Float_t CurrentValuePrecis = PwmObject.VPreciseStep;
	fmultiple(&CurrentValuePrecis, Chan->CurrentExtraSkok);

	Float_t CurrentValue = fadd_c(&CurrentValueBasic, &CurrentValuePrecis);

	return CurrentValue;
}

Float_t pwm_get_freq(uint8_t UNIT)
{
	Float_t F;
	fset(&F, t1_get_clk_freq(PwmObject.Prescaller),0);
	fdiv(&F, (PwmObject.Period+1));

	uint32_t Multipler=1;
	for(uint8_t X = 0; X<UNIT; ++X)
		Multipler*=10;

	fdiv(&F, Multipler);

	return F;
}

Float_t pwm_get_period(void)
{
	Float_t F;
	fset(&F, PwmObject.Period,0);
	return F;
}

Float_t pwm_get_duty(uint8_t Channel)
{
	Float_t F;
	fset(&F, (Channel == TimerOneChannelA) ? PwmObject.ChannelA.Duty : PwmObject.ChannelB.Duty,0);
	return F;
}

Float_t pwm_get_extra_duty(uint8_t Channel)
{
	Float_t F;
	fset(&F, (Channel == TimerOneChannelA) ? PwmObject.ChannelA.CurrentExtraSkok : PwmObject.ChannelB.CurrentExtraSkok, 0);
	return F;
}


Float_t pwm_get_basic_step(void)
{
	return PwmObject.VBasicStep;
}

Float_t pwm_get_precise_step(void)
{
	return PwmObject.VPreciseStep;
}

Float_t pwm_get_reference(void)
{
	return PwmObject.VReference;
}

void pwm_setup_channel(uint8_t Channel, uint8_t Mode)
{
	Pin PinObject;
	pin_setup(&PinObject, &DDRB, &PINB, &PORTB, ((Channel == TimerOneChannelA) ? PINB1 : PINB2));
	pin_as_output(&PinObject);
	pin_write_bit(&PinObject, 0);

	switch (Channel)
	{
	case TimerOneChannelA:
		PwmObject.ChannelA.Mode = Mode;
		break;
	case TimerOneChannelB:
		PwmObject.ChannelB.Mode = Mode;
		break;
	}

	t1_configure_channel_mode(Mode, Channel);
}

void pwm_handle_interrupt_channelA(void)
{
	PwmObject.ChannelA.ExtraDutySuma += PwmObject.ChannelA.CurrentExtraSkok;
	OCR1A = PwmObject.ChannelA.Duty + (PwmObject.ChannelA.ExtraDutySuma / PwmObject.Diver);
	PwmObject.ChannelA.ExtraDutySuma %= PwmObject.Diver;
}

void pwm_handle_interrupt_channelB(void)
{

	PwmObject.ChannelB.ExtraDutySuma += PwmObject.ChannelB.CurrentExtraSkok;
	OCR1B = PwmObject.ChannelB.Duty + (PwmObject.ChannelB.ExtraDutySuma / PwmObject.Diver);
	PwmObject.ChannelB.ExtraDutySuma %= PwmObject.Diver;
}

bool pwm_is_running(void)
{
	return ((TCCR1B & 7) > 0);
}

Float_t pwm_get_extra_period(void)
{
	Float_t X;
	fset(&X, PwmObject.Diver, 0);
	return X;
}


