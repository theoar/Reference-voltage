/*
 * variable.c
 *
 *  Created on: 13-11-2017
 *      Author: theoar
 */


#include "variable.h"
#include "hd44780.h"
#include "float_t.h"
#include "actions.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t variable_next_left(Variable* Var)
{
	Var->CurrentPos--;
	variable_calculate_position(Var);
	return Var->CurrentX;
}

uint8_t variable_next_right(Variable* Var)
{
	Var->CurrentPos++;
	variable_calculate_position(Var);
	return Var->CurrentX;
}

void variable_calculate_position(Variable *Var)
{
	if (Var->CurrentPos > Var->IntegerDigits+Var->FloatedDigits - 1)
		Var->CurrentPos = 0;

	if (Var->CurrentPos == -1)
		Var->CurrentPos = Var->IntegerDigits+Var->FloatedDigits-1;

	if (Var->CurrentPos < Var->IntegerDigits)
		Var->CurrentX = Var->Len_Prefix + Var->CurrentPos;
	else
		Var->CurrentX = Var->Len_Prefix + Var->CurrentPos + 1;

	lcd_goto_XY(Var->CurrentX, Var->Y_Pos);
}


void variable_increment(Variable* Var)
{
	if(Var->ListCount!=0)
	{
		Var->ListIndex++;
		if(Var->ListIndex==Var->ListCount)
			Var->ListIndex=0;
		Var->Value = *Var->List[Var->ListIndex];
	}
	else
	{
		Float_t Temp;
		if(Var->CurrentPos==-1)
		{
			Temp = fadd_c(&Var->Value, &Var->Step);
			if(f_is_greater(&Temp, &Var->Max))
				Var->Value=Var->Max;
			else
				fadd(&Var->Value, &Var->Step);
		}
		else
		{
			uint32_t Multipler = 1;
			for (uint8_t X = 0;	X< 4 - Var->FloatedDigits + (Var->IntegerDigits + Var->FloatedDigits - 1- Var->CurrentPos); ++X)
				Multipler *= 10;

			Float_t ObecnyStep = f_single_step();
			fmultiple(&ObecnyStep, Multipler);

			Temp = fdiv_c(&Var->Value, &ObecnyStep);

			if (f_get_integer(&Temp) % 10 == 9)
			{
				fmultiple(&ObecnyStep, 9);
				fminus(&Var->Value, &ObecnyStep);
			}
			else
				fadd(&Var->Value, &ObecnyStep);
		}
	}

	variable_display_value(Var);
}

void variable_decrement(Variable* Var)
{
	if(Var->ListCount!=0)
	{
		Var->ListIndex--;
		if(Var->ListIndex==255)
			Var->ListIndex=Var->ListCount-1;

		Var->Value = *Var->List[Var->ListIndex];
	}
	else
	{
		Float_t Temp;
		if (Var->CurrentPos == -1)
		{
			Temp = fminus_c(&Var->Value, &Var->Step);
			if (f_is_lesser(&Temp, &Var->Min))
				Var->Value = Var->Min;
			else
				fminus(&Var->Value, &Var->Step);
		}
		else
		{

			uint32_t Multipler = 1;
			for (uint8_t X = 0;	X< 4-Var->FloatedDigits+(Var->IntegerDigits+Var->FloatedDigits-1-Var->CurrentPos); ++X)
				Multipler *= 10;

			Float_t ObecnyStep = f_single_step();
			fmultiple(&ObecnyStep, Multipler);

			Temp = fdiv_c(&Var->Value, &ObecnyStep);

			if(f_get_integer(&Temp) % 10==0)
			{
				fmultiple(&ObecnyStep, 9);
				fadd(&Var->Value, &ObecnyStep);
			}
			else
				fminus(&Var->Value, &ObecnyStep);

		}
	}
	variable_display_value(Var);
}

void variable_set_values(Variable* Var, Float_t * Value, Float_t * Max,  Float_t * Min, Float_t * Step)
{
	Var->Value = *Value;
	Var->Max = *Max;
	Var->Min = *Min;
	Var->Step = *Step;

	Var->IntegerDigits=f_get_integer_digits(&Var->Max);
	Var->FloatedDigits=f_get_floated_digits(&Var->Step);

	Var->Len_Var =  Var->IntegerDigits + (Var->FloatedDigits > 0 ? Var->FloatedDigits + 1 : 0);
	Var->CurrentPos = -1;
}

void variable_set_list(Variable *Var, Float_t ** List, const uint8_t Count)
{
	Var->List=List;
	Var->ListIndex=0;
	Var->ListCount=Count;

	Var->Value = *Var->List[0];
}

void variable_set_index(Variable * Var, uint8_t Index)
{
	Var->ListIndex=Index;
	Var->Value = *Var->List[Var->ListIndex];
}

uint8_t variable_get_index(Variable * Var)
{
	return Var->ListIndex;
}

void variable_display_value(Variable* Var)
{
	char Buff[32];
	variable_goto_value(Var);

	uint8_t CurrentIntegerDigits = f_get_integer_digits(&Var->Value);
	for(uint8_t X = CurrentIntegerDigits; X<Var->IntegerDigits; ++X)
		lcd_putc('0');

	if(CurrentIntegerDigits!=0)
	{
		itoa(f_get_integer(&Var->Value),Buff,10);
		lcd_puts(Buff);
	}

	if(Var->FloatedDigits>0)
	{
		lcd_putc('.');

		if(f_is_zero(&Var->Value))
		{
			for(uint8_t X = 0; X<Var->FloatedDigits; ++X)
				lcd_putc('0');
		}
		else
		{
			Float_t Temp;
			fset(&Temp, 1 ,0);
			fadd(&Temp, f_get_floated(&Var->Value));
			ltoa(f_get_mod(&Temp),Buff, 10);
			Buff[Var->FloatedDigits+1] = '\0';
			lcd_puts(&Buff[1]);
		}
	}

	variable_goto_current(Var);
}

void variable_display_prefix(Variable* Var)
{
	variable_goto_prefix(Var);
	lcd_puts(Var->Prefix);
}

void variable_goto_current(Variable* Var)
{
	if(variable_is_actived_edit(Var))
		lcd_goto_XY(Var->CurrentX,  Var->Y_Pos);
}

void variable_set_prefix(Variable *Var, const char *Prefix)
{

	strcpy_P(Var->Prefix, Prefix);

	uint8_t X = 0;
	while(Var->Prefix[X]!='\0')
		X++;

	Var->Len_Prefix = X;
}

void variable_set_sufix(Variable* Var, const  char *Sufix)
{

	strcpy_P(Var->Sufix, Sufix);

	uint8_t X = 0;
	while (Var->Sufix[X] != '\0')
		X++;

	Var->Len_Sufix = X;
}

void variable_set_title(Variable* Var, const char *Title)
{
	strcpy_P(Var->Title, Title);
}

void variable_display_sufix(Variable *Var)
{
	variable_goto_sufix(Var);
	lcd_puts(Var->Sufix);
}

void variable_goto_title(Variable *Var)
{
	lcd_goto_XY(0,0);
}

void variable_display_title(Variable *Var)
{
	lcd_clr_row(0);
	variable_goto_title(Var);
	lcd_puts(Var->Title);
}

void variable_active(Variable* Var)
{
	Var->CurrentPos = -1;
}

void variable_display(Variable *Var)
{
	lcd_clrscr();
	variable_display_title(Var);
	variable_display_prefix(Var);
	variable_display_value(Var);
	variable_display_sufix(Var);
}

void variable_active_edit(Variable *Var)
{
	if(Var->CurrentPos==-1 && Var->ListCount==0)
	{
		Var->CurrentPos = 0;
		variable_calculate_position(Var);
		lcd_enable_cursor();
	}
}

void variable_goto_prefix(Variable *Var)
{
	lcd_goto_XY(Var->X_Pos, Var->Y_Pos);
}

void variable_goto_value(Variable *Var)
{
	lcd_goto_XY(Var->Len_Prefix, Var->Y_Pos);
}
void variable_goto_sufix(Variable *Var)
{
	lcd_goto_XY(Var->Len_Prefix+Var->Len_Var, Var->Y_Pos);
}

void variable_display_variable(Variable* Var)
{
	variable_goto_prefix(Var);
	for(uint8_t X = 0; X<15; ++X)
		lcd_putc(' ');

	variable_display_prefix(Var);
	variable_display_value(Var);
	variable_display_sufix(Var);

	if(variable_is_actived_edit(Var))
		variable_goto_current(Var);
}

void variable_accept(Variable *Var)
{
	if(Var->ListCount==0)
	{
		Float_t Temp = fdiv_c(&Var->Value, &Var->Step);
		Var->Value = fmultiple_c(&Var->Step, f_get_integer(&Temp));

		if (f_is_greater(&Var->Value, &Var->Max))
			Var->Value = Var->Max;
		else if (f_is_lesser(&Var->Value, &Var->Min))
			Var->Value = Var->Min;

		variable_display_value(Var);
		variable_deactive_edit(Var);
	}
}

void variable_deactive_edit(Variable *Var)
{
	Var->CurrentPos = -1;
	lcd_disable_cursor();
}

bool variable_is_actived_edit(Variable *Var)
{
	return Var->CurrentPos > -1;
}

void variable_readonly(Variable *Var, bool ReadOnly)
{
	Var->ReadOnly = ReadOnly;
}

bool variable_is_readonly(Variable *Var)
{
	return Var->ReadOnly;
}

uint8_t variable_handle_action(Variable *Var, uint8_t Action)
{
	if(variable_is_readonly(Var))
		return 0;

	switch(Action)
	{
	case IO_ACTION_POS_LEFT:
		if(variable_is_actived_edit(Var))
			variable_next_left(Var);
		break;
	case IO_ACTION_POS_RIGHT:
		if(variable_is_actived_edit(Var))
			variable_next_right(Var);
		break;
	case IO_ACTION_VAL_LEFT:
		variable_decrement(Var);
		if(!variable_is_actived_edit(Var))
			return Var->AccpetEvent;
		break;
	case IO_ACTION_VAL_RIGHT:
		variable_increment(Var);
		if(!variable_is_actived_edit(Var))
			return Var->AccpetEvent;
		break;
	case IO_ACTION_ENTER:
		variable_active_edit(Var);
		break;
	case IO_ACTION_ESCAPE:
		variable_accept(Var);
		return Var->AccpetEvent;
		break;
	}

	return 0;
}

void variable_set_acept_event(Variable *Var, uint8_t Event)
{
	Var->AccpetEvent = Event;
}

void variable_set_value(Variable* Var, const Float_t * Value)
{
	if(!(f_is_lesser(Value, &Var->Min) || f_is_greater(Value, &Var->Max)))
		Var->Value = *Value;
}

void variable_set_value(Variable *Var, const Float_t Value)
{
	if(!(f_is_lesser(&Value, &Var->Min) || f_is_greater(&Value, &Var->Max)))
			Var->Value = Value;
}

