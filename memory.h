/*
 * memory.h
 *
 *  Created on: 22-11-2017
 *      Author: theoar
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "float_t.h"
#include <avr/eeprom.h>

#define MEMORY_ADRESS_BEGIN (0)
#define MEMORY_ADRESS_REF (MEMORY_ADRESS_BEGIN+sizeof(Float_t))
#define MEMORY_ADRESS_STEP (MEMORY_ADRESS_REF+sizeof(Float_t))
#define MEMORY_ADRESS_PSTEP (MEMORY_ADRESS_STEP+sizeof(Float_t))
#define MEMORY_ADRESS_VOLTAGE (MEMORY_ADRESS_PSTEP+sizeof(Float_t))
#define MEMORY_ADRESS_MODE (MEMORY_ADRESS_VOLTAGE+sizeof(Float_t))
#define MEMORY_ADRESS_PRESCALLER (MEMORY_ADRESS_MODE+sizeof(Float_t))
#define MEMORY_ADR(X) ((void*)X)

const char TitleVerseOne[]  PROGMEM = "   #Voltage#   ";
const char TitleVerseTwo[]  PROGMEM = "  #Reference#  ";
const char TitleRestoringOne[] PROGMEM = "####Settings####";
const char TitleRestroingTwo[] PROGMEM = "####Restoring###";

const char TitleVarSettingsReference[] PROGMEM = "Reference";
const char TitleVarSettingsPStep[] PROGMEM = "Precise Step";
const char TitleVarSettingsStep[] PROGMEM = "Basic Step";
const char TitleVarSettingsPrescsaller[] PROGMEM = "Prescaller";
const char TitleVarSettingsMode[] PROGMEM = "Operation Mode";

const char TitleVarVersion[] PROGMEM = "Version";
const char TitleOutputVoltage[] PROGMEM = "Output Voltage";

const char TitleVarPwmFreq[] PROGMEM = "Frequency";
const char TitleVarPwmPeriod[] PROGMEM = "Period";
const char TitleVarPwmDuty[]  PROGMEM = "Duty";
const char TitleVarPwmEDuty[]  PROGMEM = "Extra Duty";
const char TitleVarPwmEPeriod[] PROGMEM  = "Extra Period";

const char TitleSettingsOperationsAccept[] PROGMEM  = "Accept";
const char TitleSettingsOperationsAbort[] PROGMEM  = "Abort";
const char TitleSettingsOperationsReset[] PROGMEM  = "Reset";
const char TitleSettingsOperationsSettings[] PROGMEM  = "Settings";

const char TitleSettingsOperations[] PROGMEM  =  "Settings";
const char TitleMainOperations[] PROGMEM =  "Configuration";

const char TitleParametersOperationsExit[] PROGMEM =   "Exit";
const char TitleParametersOperations[] PROGMEM = "Parameters";

const char TitleSettingsMenu[] PROGMEM =  "Settings";
const char TitleParametersMenu[] PROGMEM =  "Parameters";
const char TitleAboutMenu[] PROGMEM =  "About";

const char PrefixVarPwmFreq[] PROGMEM = "f = ";
const char SufixVarPwmFreq[] PROGMEM = " KHz";

const char EmptyString[] PROGMEM = "";
const char SufixVolt[] PROGMEM = " V";
const char PrefixNo[] PROGMEM = "No. = ";

#define SufixVarPwmPeriod EmptyString
#define SufixVarPwmDuty EmptyString
#define SufixVarPwmEDuty EmptyString
#define SufixVarPwmEPeriod EmptyString
#define SufixVarVersion EmptyString
#define SufixVarSettingsMode EmptyString
#define SufixVarSettingsPrescaller EmptyString

#define SufixVarSettingsReference SufixVolt
#define SufixVarSettingsPStep SufixVolt
#define SufixVarSettingsStep SufixVolt
#define SufixOutputVoltage SufixVolt

#define PrefixVarPwmPeriod PrefixNo
#define PrefixVarPwmDuty PrefixNo
#define PrefixVarPwmEDuty PrefixNo
#define PrefixVarPwmEPeriod PrefixNo

const char PrefixOutputVoltage[] PROGMEM =  "V = ";

const char PrefixVarVersion[] PROGMEM = "Rev: ";

const char PrefixVarSettingsReference[] PROGMEM = "Vr = ";
const char PrefixVarSettingsPStep[] PROGMEM = "Vp = ";
const char PrefixVarSettingsStep[] PROGMEM = "Vb = ";
const char PrefixVarSettingsMode[] PROGMEM = "Mode: ";
const char PrefixVarSettingsPrescaller[] PROGMEM = "Cpu/";



void memory_write(Float_t *Source, void* Dest);
void memory_read(Float_t *Dest, void* Source);
void memory_write(uint8_t *Source, void* Dest);
void memory_read(uint8_t *Dest, void* Source);

void memory_write(Float_t *Source, void* Dest)
{
	eeprom_write_block((void*)Source, Dest, sizeof(Float_t));
}

void memory_read(Float_t *Dest, void* Source)
{
	eeprom_read_block((void*)Dest, Source, sizeof(Float_t));
}

void memory_write(uint8_t *Source, void* Dest)
{
	eeprom_write_block((void*)Source, Dest, sizeof(uint8_t));
}

void memory_read(uint8_t *Dest, void* Source)
{
	eeprom_read_block((void*)Dest, Source, sizeof(uint8_t));
}


#endif /* MEMORY_H_ */
