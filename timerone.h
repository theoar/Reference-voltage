/*
 * timerone.h
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#ifndef TIMERONE_H_
#define TIMERONE_H_

#include <avr/io.h>

#define OCR1A_PORT PORTB
#define OCR1B_PORT PORTB
#define OCR1A_PIN PINB1
#define OCR1B_PIN PINB2
#define OCR1A_DDRX DDRB
#define OCR1B_DDRX DDRB

enum TimeOnePrescallerEnum{
	TimerOnePrescaller1 = 1,
	TimerOnePrescaller8 = 2,
	TimerOnePrescaller64 = 3,
	TimerOnePrescaller256 = 4,
	TimerOnePrescaller1024 = 5,
	TimerOneExternalFalling = 6,
	TimerOneExternalRising = 7
};

enum TimerOneModesEnum{
	TimerOneNormal = 0,
	TimerOnePhaseCorrect8b = 1,
	TimerOnePhaseCorrect9b = 2,
	TimerOnePhaseCorrect10b = 3,
	TimerOneCtcOCR1A = 4,
	TimerOneFastPwm8b = 5,
	TimerOneFastPwm9b = 6,
	TimerOneFastPwm10b = 7,
	TimerOnePhaseFreqCorrectICR1 = 8,
	TimerOnePhaseFreqCorrectOCR1A = 9,
	TimerOnePhaseICR1 = 10,
	TimerOnePhaseOCR1A = 11,
	TimerOneCtcICR1 = 12,
	TimerOneFastPwmICR1 = 14,
	TimerOneFastPwmOCR1A = 15,
};

enum TimerOneChannelsEnum{
	TimerOneChannelA = 1,
	TimerOneChannelB = 2
};

enum TimerOneChannelsModesEnum{
	//odlacz
	DisconnectChannel = 0,

	//wszystkie tryby
	DisconnectOC1A = 0,
	DisconnectOC1B = 0,

	//tryb compare output mode, non pwm
	ToggleOC1AOnCompare = (1<<2),
	ToggleOC1BOnCompare = 1,

	ClearOCA1OnCompare = (2<<2),
	ClearOCB1OnCompare = 2,

	SetOCA1OnCompare = (3<<2),
	SetOCB1OnCompare = (3),

	//tryb compare output mode, fast pwm
	ClearOCA1OnCompareSetAtBottom = 2,
	ClearOCB1AOnCompareSetAtBottom = 2,

	SetOCA1OnCompareClearAtBottom = 3,
	SetOCB1OnCompareClearAtBottom = 3,

	//tryb compare output, phase correct
	ClearOCA1OnCompareUpContSetAtDownCont = 2,
	ClearOCB1OnCompareUpContSetAtDownCont = 2,

	SetOCA1AOnCompareUpContClearAtDownCont = 3,
	SetOCB1AOnCompareUpContClearAtDownCont = 3,
};

enum TimerOneInterruptsEnum{
	OverflowInterrupt = 1,
	OutputCompareAInterrupt = 2,
	OutputCompareBInterrupt = 4,
	InputCaputreInterrupt = 32
};


void t1_configure_mode(uint8_t Mode);
void t1_configure_channel_mode(uint8_t Tryb, uint8_t Channel);
void t1_start(uint8_t Prescaller);
void t1_stop(void);
void t1_enable_interrupts(uint8_t Interupts);
void t1_disable_interrupts(uint8_t Interupts);

uint32_t t1_get_clk_freq(uint8_t Prescaller);

void set_period(uint16_t Period);
void set_duty(uint8_t Channel, uint16_t Duty);

uint8_t t1_get_channel_mode(uint8_t Channel);


#endif /* TIMERONE_H_ */
