/*
 * main.c
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>

#include "precisepwm.h"
#include "timerone.h"
#include "float_t.h"
#include "pin.h"
#include "uart_bus.h"
#include "hd44780.h"
#include "io_events.h"
#include "variable.h"
#include "enkoder.h"

#include "options.h"
#include "lowermenu.h"
#include "actions.h"
#include "main_menu.h"

#include "memory.h"


int main(void)
{

	init_uart();
	lcd_init();
	pwm_init();
	io_events_init();
	io_events_add_my_io();

	lcd_goto_XY(0,0);
	lcd_puts_P(TitleVerseOne);
	lcd_goto_XY(0, 1);
	lcd_puts_P(TitleVerseTwo);

	sei();

	Float_t BasicRef;
	fset(&BasicRef, 5, 0000);
	Float_t BasicStep;
	fset(&BasicStep, 0, 100);
	Float_t BasicPStep;
	fset(&BasicPStep, 0, 10);
	Float_t BasicMode;
	fset(&BasicMode, 1, 0000);
	Float_t Max;
	fset(&Max, 4, 9000);
	Float_t Min;
	fset(&Min, 0 ,1000);

	uint8_t BasicPreIndex = 1;

	Float_t Voltage;

	Float_t LoadedRef;
	Float_t LoadedStep;
	Float_t LoadedPStep;
	Float_t LoadedMode;
	uint8_t LoadedPreIndex;

	Float_t GlobalZero;
	fset(&GlobalZero, 0, 0);

	Float_t GlobalMin;
	fset(&GlobalMin, 0, 1);

	Float_t GlobalStep;
	fset(&GlobalStep, 0, 1);

	Float_t GlobalMax;
	fset(&GlobalMax, 9999, 9999);

	Float_t GlobalMinRef;
	fset(&GlobalMinRef, 1, 0);

	Float_t GlobalMaxRef;
	fset(&GlobalMaxRef, 5, 0);

	Float_t GlobalMaxPStep;
	fset(&GlobalMaxPStep, 0, 100);

	Float_t GlobalMaxStep;
	fset(&GlobalMaxStep, 0, 1000);

	Float_t GlobalFreqPrec;
	fset(&GlobalFreqPrec, 0, 10);

	Float_t GlobalVersion;
	fset(&GlobalVersion, 1, 1569);

	Float_t GlobalOne;
	fset(&GlobalOne, 1, 0);

	Float_t GlobalTwo;
	fset(&GlobalTwo, 2, 0);


	_delay_ms(2000);
	uint16_t Event = io_events_read();
	if( ((Event & (1 << KEYA_PUSH)) | (Event & (1 << KEYB_PUSH))) == ((1 << KEYA_PUSH) | (1 << KEYB_PUSH)) )
	{
		LoadedRef = BasicRef;
		LoadedStep = BasicStep;
		LoadedPStep = BasicPStep;
		LoadedMode = BasicMode;
		LoadedPreIndex = BasicPreIndex;
		lcd_clrscr();
		lcd_goto_XY(0,0);
		lcd_puts_P(TitleRestoringOne);
		lcd_goto_XY(0, 1);
		lcd_puts_P(TitleRestroingTwo);
	}
	else
	{
		memory_read(&LoadedRef, MEMORY_ADR(MEMORY_ADRESS_REF));
		memory_read(&LoadedStep, MEMORY_ADR(MEMORY_ADRESS_STEP));
		memory_read(&LoadedPStep, MEMORY_ADR(MEMORY_ADRESS_PSTEP));
		memory_read(&LoadedMode, MEMORY_ADR(MEMORY_ADRESS_MODE));
		memory_read(&LoadedPreIndex, MEMORY_ADR(MEMORY_ADRESS_PRESCALLER));

	}
	_delay_ms(2000);


	Voltage = pwm_setup(&LoadedRef, &LoadedStep, &LoadedPStep, LoadedPreIndex+1);
	pwm_setup_channel(TimerOneChannelA, ClearOCA1OnCompareSetAtBottom);

	if(f_is_equal(&LoadedMode, &GlobalOne))
		pwm_setup_channel(TimerOneChannelA, ClearOCA1OnCompareSetAtBottom);
	if(f_is_equal(&LoadedMode, &GlobalZero))
		pwm_setup_channel(TimerOneChannelA, SetOCA1OnCompareClearAtBottom);

	Max = LoadedRef;
	Min = BasicStep;
	fmultiple(&Min, 10);
	fminus(&Max, &Min);

	const uint8_t PrescallerCount = 5;
	Float_t *PrescallerValues[PrescallerCount];
	Float_t Pre1;
	Float_t Pre8;
	Float_t Pre64;
	Float_t Pre256;
	Float_t Pre1024;

	fset(&Pre1, 1, 0);
	fset(&Pre8, 8, 0);
	fset(&Pre64, 64, 0);
	fset(&Pre256, 256, 0);
	fset(&Pre1024, 1024, 0);

	PrescallerValues[0] = &Pre1;
	PrescallerValues[1] = &Pre8;
	PrescallerValues[2] = &Pre64;
	PrescallerValues[3] = &Pre256;
	PrescallerValues[4] = &Pre1024;

	Float_t SettingsReference = LoadedRef;
	Float_t SettingsPStep = LoadedPStep;
	Float_t SettingsStep = LoadedStep;
	Float_t SettingsMode = LoadedMode;
	Float_t SettingsPrecsallerIndex = *PrescallerValues[LoadedPreIndex];
	uint8_t SettingdPrescallerIndexInt = LoadedPreIndex;

	Variable VarSettingsReference;
	Variable VarSettingsPStep;
	Variable VarSettingsStep;
	Variable VarSettingsMode;
	Variable VarSettingsPrescaller;

	variable_set_values(&VarSettingsReference, &SettingsReference, &GlobalMaxRef, &GlobalMinRef, &GlobalMin);
	variable_set_values(&VarSettingsPStep, &SettingsPStep, &GlobalMaxPStep, &GlobalMin, &GlobalMin);
	variable_set_values(&VarSettingsStep, &SettingsStep, &GlobalMaxStep, &GlobalMin, &GlobalMin);
	variable_set_values(&VarSettingsMode, &SettingsMode, &GlobalOne, &GlobalZero, &GlobalOne);
	variable_set_values(&VarSettingsPrescaller, &SettingsPrecsallerIndex, &GlobalMax, &GlobalZero, &GlobalOne);

	variable_set_list(&VarSettingsPrescaller, PrescallerValues, PrescallerCount);
	variable_set_index(&VarSettingsPrescaller, SettingdPrescallerIndexInt);

	variable_set_title(&VarSettingsReference, TitleVarSettingsReference);
	variable_set_title(&VarSettingsPStep, TitleVarSettingsPStep );
	variable_set_title(&VarSettingsStep, TitleVarSettingsStep);
	variable_set_title(&VarSettingsMode, TitleVarSettingsMode);
	variable_set_title(&VarSettingsPrescaller, TitleVarSettingsPrescsaller);

	variable_set_prefix(&VarSettingsReference, PrefixVarSettingsReference);
	variable_set_prefix(&VarSettingsPStep, PrefixVarSettingsPStep);
	variable_set_prefix(&VarSettingsStep, PrefixVarSettingsStep);
	variable_set_prefix(&VarSettingsMode, PrefixVarSettingsMode);
	variable_set_prefix(&VarSettingsPrescaller, PrefixVarSettingsPrescaller);

	variable_set_sufix(&VarSettingsReference, SufixVarSettingsReference);
	variable_set_sufix(&VarSettingsPStep, SufixVarSettingsPStep);
	variable_set_sufix(&VarSettingsStep, SufixVarSettingsStep);
	variable_set_sufix(&VarSettingsMode, SufixVarSettingsMode);
	variable_set_sufix(&VarSettingsPrescaller, SufixVarSettingsPrescaller);

	LowerMenu SettingsMenu;
	lm_add_variable(&SettingsMenu, &VarSettingsReference);
	lm_add_variable(&SettingsMenu, &VarSettingsStep);
	lm_add_variable(&SettingsMenu, &VarSettingsPStep);
	lm_add_variable(&SettingsMenu, &VarSettingsMode);
	lm_add_variable(&SettingsMenu, &VarSettingsPrescaller);

	Options SettingsOperations;
	options_add_option(&SettingsOperations, TitleSettingsOperationsAccept, ACTION_NEW_PWM_SETTINGS);
	options_add_option(&SettingsOperations, TitleSettingsOperationsAbort, ACTION_ABORT_SETTINGS);
	options_add_option(&SettingsOperations, TitleSettingsOperationsReset, ACTION_RESET_PWM_SETTINGS);
	options_add_title(&SettingsOperations, TitleSettingsOperationsSettings);

	lm_add_action_menu(&SettingsMenu, &SettingsOperations);


	Float_t PwmFreq = pwm_get_freq(KHZ);
	Float_t PwmPeriod = pwm_get_period();
	Float_t PwmDuty = pwm_get_duty(TimerOneChannelA);
	Float_t PwmEDuty = pwm_get_extra_duty(TimerOneChannelA);
	Float_t PwmEPeriod = pwm_get_extra_period();

	Variable VarPwmFreq;
	Variable VarPwmPeriod;
	Variable VarPwmDuty;
	Variable VarPwmEDuty;
	Variable VarPwmEPeriod;

	variable_readonly(&VarPwmFreq, true);
	variable_readonly(&VarPwmPeriod, true);
	variable_readonly(&VarPwmDuty, true);
	variable_readonly(&VarPwmEDuty, true);
	variable_readonly(&VarPwmEPeriod, true);

	variable_set_values(&VarPwmFreq, &PwmFreq, &GlobalMax, &GlobalZero, &GlobalFreqPrec);
	variable_set_values(&VarPwmPeriod, &PwmPeriod, &GlobalMax, &GlobalZero, &GlobalOne);
	variable_set_values(&VarPwmDuty, &PwmDuty, &GlobalMax, &GlobalZero, &GlobalOne);
	variable_set_values(&VarPwmEDuty, &PwmEDuty, &GlobalMax, &GlobalZero, &GlobalOne);
	variable_set_values(&VarPwmEPeriod, &PwmEPeriod, &GlobalMax, &GlobalZero, &GlobalOne);

	variable_set_title(&VarPwmFreq, TitleVarPwmFreq);
	variable_set_title(&VarPwmPeriod, TitleVarPwmPeriod);
	variable_set_title(&VarPwmDuty, TitleVarPwmDuty);
	variable_set_title(&VarPwmEDuty, TitleVarPwmEDuty);
	variable_set_title(&VarPwmEPeriod, TitleVarPwmEPeriod);

	variable_set_prefix(&VarPwmFreq, PrefixVarPwmFreq);
	variable_set_prefix(&VarPwmPeriod, PrefixVarPwmPeriod);
	variable_set_prefix(&VarPwmDuty, PrefixVarPwmDuty);
	variable_set_prefix(&VarPwmEDuty, PrefixVarPwmEDuty);
	variable_set_prefix(&VarPwmEPeriod, PrefixVarPwmEPeriod);

	variable_set_sufix(&VarPwmFreq, SufixVarPwmFreq);
	variable_set_sufix(&VarPwmPeriod, SufixVarPwmPeriod);
	variable_set_sufix(&VarPwmDuty, SufixVarPwmDuty);
	variable_set_sufix(&VarPwmEDuty, SufixVarPwmEDuty);
	variable_set_sufix(&VarPwmEPeriod, SufixVarPwmEPeriod);

	LowerMenu ParametersMenu;
	lm_add_variable(&ParametersMenu, &VarPwmFreq);
	lm_add_variable(&ParametersMenu, &VarPwmPeriod);
	lm_add_variable(&ParametersMenu, &VarPwmDuty);
	lm_add_variable(&ParametersMenu, &VarPwmEPeriod);
	lm_add_variable(&ParametersMenu, &VarPwmEDuty);

	Options ParametersOperations;
	options_add_option(&ParametersOperations, TitleParametersOperationsExit, ACTION_GO_UP);
	options_add_title(&ParametersOperations, TitleParametersOperations);

	lm_add_action_menu(&ParametersMenu, &ParametersOperations);

	Variable VarVersion;
	variable_set_values(&VarVersion, &GlobalVersion, &GlobalMax, &GlobalZero, &GlobalStep);
	variable_set_title(&VarVersion, TitleVarVersion);
	variable_set_prefix(&VarVersion, PrefixVarVersion);
	variable_set_sufix(&VarVersion, SufixVarVersion);
	variable_readonly(&VarVersion, true);

	LowerMenu AboutMenu;
	lm_add_variable(&AboutMenu, &VarVersion);

	Options AboutOperations;
	options_add_option(&AboutOperations, TitleParametersOperationsExit, ACTION_GO_UP);
	options_add_title(&AboutOperations, TitleParametersOperations);

	lm_add_action_menu(&AboutMenu, &AboutOperations);

	Variable OutputVoltage;
	variable_set_values(&OutputVoltage, &Voltage, &Max, &Min, &BasicPStep);
	variable_set_prefix(&OutputVoltage, PrefixOutputVoltage);
	variable_set_sufix(&OutputVoltage, SufixOutputVoltage);
	variable_set_title(&OutputVoltage, TitleOutputVoltage);
	variable_set_acept_event(&OutputVoltage, ACTION_NEW_VOLTAGE);

	Options MainOperations;
	options_add_title(&MainOperations, TitleMainOperations);

	MainMenu Main;
	mm_add_variable(&Main, &OutputVoltage);
	mm_add_options(&Main, &MainOperations);

	mm_add_lower_menu(&Main, &SettingsMenu, TitleSettingsMenu);
	mm_add_lower_menu(&Main, &ParametersMenu, TitleParametersMenu);
	mm_add_lower_menu(&Main, &AboutMenu, TitleAboutMenu);

	mm_display(&Main);

	pwm_start();

	uint8_t Action = 0;
	while (1)
	{
		Event = io_events_read();
		for (uint8_t X = 0; X < 8; ++X)
		{
			switch (Event & (1 << X))
			{
			case (1 << ENKODERA_LEFT): //prawo lewo o menu
			{
				Action = mm_handle_action(&Main, IO_ACTION_POS_LEFT);
			}
			break;
			case (1 << ENKODERA_RIGHT):
			{
				Action = mm_handle_action(&Main, IO_ACTION_POS_RIGHT);

			}
			break;
			case (1 << ENKODERB_LEFT): //prawo lewo wartosc
			{
				Action = mm_handle_action(&Main, IO_ACTION_VAL_LEFT);
			}
			break;
			case (1 << ENKODERB_RIGHT):
			{
				Action = mm_handle_action(&Main, IO_ACTION_VAL_RIGHT);
			}
			break;
			case (1 << KEYA_PUSH): //wybierz - wejdz glebiej (wybierz)
			{
				Action = mm_handle_action(&Main, IO_ACTION_ENTER);

			}
			break;
			case (1 << KEYB_PUSH): //wstecz - koniec edycji zmiennej
			{
				Action = mm_handle_action(&Main, IO_ACTION_ESCAPE);
			}
			break;
			}
			Event &= ~(1 << X);
		}

		switch(Action)
		{
		case ACTION_NEW_VOLTAGE:
			pwm_set_channelA(OutputVoltage.Value);
			variable_set_value(&VarPwmDuty, pwm_get_duty(TimerOneChannelA));
			variable_set_value(&VarPwmEDuty, pwm_get_extra_duty(TimerOneChannelA));
			variable_set_value(&OutputVoltage, pwm_get_curent(TimerOneChannelA));
			mm_display_main(&Main);
		break;
		case ACTION_ABORT_SETTINGS:
			//printf("abort\n");
			variable_set_value(&VarSettingsReference, &SettingsReference);
			variable_set_value(&VarSettingsPStep, &SettingsPStep);
			variable_set_value(&VarSettingsStep, &SettingsStep);
			variable_set_value(&VarSettingsMode, &SettingsMode);
			variable_set_index(&VarSettingsPrescaller, SettingdPrescallerIndexInt);
			mm_display_main(&Main);
		break;
		case ACTION_RESET_PWM_SETTINGS:
			//printf("rst\n");
			variable_set_value(&VarSettingsReference, &BasicRef);
			variable_set_value(&VarSettingsPStep, &BasicPStep);
			variable_set_value(&VarSettingsStep, &BasicStep);
			variable_set_value(&VarSettingsMode, &BasicMode);
			variable_set_index(&VarSettingsPrescaller, BasicPreIndex);
		case ACTION_NEW_PWM_SETTINGS:
			//printf("new set\n");
			fset(&SettingsReference, &VarSettingsReference.Value);
			fset(&SettingsPStep, &VarSettingsPStep.Value);
			fset(&SettingsStep, &VarSettingsStep.Value);
			SettingdPrescallerIndexInt = variable_get_index(&VarSettingsPrescaller);

			Max = VarSettingsReference.Value;
			Min = VarSettingsStep.Value;
			fmultiple(&Min, 10);
			fminus(&Max, &Min);

			memory_write(&VarSettingsReference.Value, MEMORY_ADR(MEMORY_ADRESS_REF));
			memory_write(&VarSettingsPStep.Value, MEMORY_ADR(MEMORY_ADRESS_PSTEP));
			memory_write(&VarSettingsStep.Value, MEMORY_ADR(MEMORY_ADRESS_STEP));
			memory_write(&VarSettingsMode.Value, MEMORY_ADR(MEMORY_ADRESS_MODE));
			memory_write(&SettingdPrescallerIndexInt, MEMORY_ADR(MEMORY_ADRESS_PRESCALLER));

			Voltage = pwm_setup(&SettingsReference, &SettingsStep, &SettingsPStep, SettingdPrescallerIndexInt+1);

			if(f_is_equal(&VarSettingsMode.Value, &GlobalOne))
				pwm_setup_channel(TimerOneChannelA, ClearOCA1OnCompareSetAtBottom);
			if(f_is_equal(&VarSettingsMode.Value, &GlobalZero))
				pwm_setup_channel(TimerOneChannelA, SetOCA1OnCompareClearAtBottom);

			variable_set_values(&OutputVoltage, &Voltage, &Max, &Min, &SettingsPStep);

			variable_set_value(&OutputVoltage, pwm_get_curent(TimerOneChannelA));
			variable_set_value(&VarPwmPeriod, pwm_get_period());
			variable_set_value(&VarPwmDuty, pwm_get_duty(TimerOneChannelA));
			variable_set_value(&VarPwmEDuty, pwm_get_extra_duty(TimerOneChannelA));
			variable_set_value(&VarPwmFreq, pwm_get_freq(KHZ));
			variable_set_value(&VarPwmEPeriod, pwm_get_extra_period());

			mm_display_main(&Main);

			pwm_start();

		break;
		}

		Action = 0;
	}

	return 0;
}
