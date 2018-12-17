/*
 * timerone.c
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */
#include <avr/io.h>

#include "timerone.h"

void t1_configure_mode(uint8_t Mode)
{
	TCCR1A = (TCCR1A & ~(3)) | (Mode & (3));
	TCCR1B = (TCCR1B & ~(3<<3)) | ((Mode & (3<<2)) << 1 );
}

void t1_configure_channel_mode(uint8_t Tryb, uint8_t Channel)
{
	TCCR1A = (TCCR1A & ~(192>>((Channel-1)*2))) | (Tryb << (6-((Channel-1)*2)));
}

uint8_t t1_get_channel_mode(uint8_t Channel)
{
	return (TCCR1A & (192>>((Channel-1)*2))) >> (6-((Channel-1)*2));
}

void t1_start(uint8_t Prescaller)
{
	TCNT1 = 0x0;
	TCCR1B = (TCCR1B & ~(7)) | (Prescaller);
}

void t1_stop(void)
{
	TCCR1B &= ~(7);
	TCNT1 = 0;
}

uint32_t t1_get_clk_freq(uint8_t Prescaller)
{
	uint16_t TimerOnePrescallerValue[5] = {1, 8, 64, 256, 1024};
	return F_CPU/TimerOnePrescallerValue[Prescaller-1];
}

void t1_enable_interrupts(uint8_t Interupts)
{
	TIMSK1 |= Interupts;
}

void t1_disable_interrupts(uint8_t Interupts)
{
	TIMSK1 &= ~(Interupts);
}

void set_period(uint16_t Period)
{
	uint8_t Mode = (TCCR1A & 3) | ((TCCR1B >> 3) & 3 );

	if(Mode == TimerOneCtcOCR1A || Mode == TimerOnePhaseFreqCorrectOCR1A || Mode == TimerOnePhaseOCR1A || Mode == TimerOneFastPwmOCR1A)
		OCR1A = Period;
	else if(Mode == TimerOnePhaseFreqCorrectICR1 || Mode == TimerOnePhaseICR1 || Mode == TimerOneCtcICR1 || Mode == TimerOneFastPwmICR1 )
		ICR1 = Period;
}

void set_duty(uint8_t Channel, uint16_t Duty)
{
	if(Channel == TimerOneChannelA)
		OCR1A = Duty;
	else if(Channel == TimerOneChannelB)
		OCR1B = Duty;
}
