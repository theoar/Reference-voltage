/*
 * options.cpp
 *
 *  Created on: 21-11-2017
 *      Author: theoar
 */
#include <avr/pgmspace.h>

#include "options.h"
#include "actions.h"
#include "functions.h"
#include "hd44780.h"

void options_add_title(Options * Opt, const char *Title)
{
	strcpy_P(Opt->Title, Title);
}

void options_add_option(Options * Opt, const char *Name, uint8_t AcceptEvent)
{
	strcpy_P(Opt->OptionNames[Opt->Count], Name);
	Opt->AcceptEvent[Opt->Count] = AcceptEvent;

	Opt->Count++;
}

void options_display_title(Options *Opt)
{
	lcd_clr_row(0);
	lcd_goto_XY(0,0);
	lcd_puts(Opt->Title);
}

void options_display_option(Options *Opt)
{
	lcd_clr_row(1);
	lcd_goto_XY((16-str_len(Opt->OptionNames[Opt->Index])-2)/2,1);

	lcd_putc(Opt->LeftArrow);
	lcd_puts(Opt->OptionNames[Opt->Index]);
	lcd_putc(Opt->RightArrow);
}

void options_display(Options *Opt)
{
	options_display_title(Opt);
	options_display_option(Opt);
}

void options_go_left(Options *Opt)
{
	if(Opt->Index < 1)
		Opt->Index = Opt->Count-1;
	else
		Opt->Index--;

	options_display_option(Opt);
}

void options_go_right(Options *Opt)
{
	if(Opt->Index+1== Opt->Count)
		Opt->Index = 0;
	else
		Opt->Index++;

	options_display_option(Opt);
}

uint8_t options_select_action(Options *Opt)
{
	return Opt->AcceptEvent[Opt->Index];
}

uint8_t options_handle_action(Options *Opt, uint8_t Action)
{
	switch (Action)
	{
	case IO_ACTION_POS_LEFT:
		return 0;
		break;
	case IO_ACTION_POS_RIGHT:
		return 0;
		break;
	case IO_ACTION_VAL_LEFT:
		options_go_left(Opt);
		return 0;
		break;
	case IO_ACTION_VAL_RIGHT:
		options_go_right(Opt);
		return 0;
		break;
	case IO_ACTION_ENTER:
		return options_select_action(Opt);
		break;
	case IO_ACTION_ESCAPE:
		return 0;
		break;
	}
	return 0;
}

void options_go_first(Options *Opt)
{
	Opt->Index = 0;
	options_display(Opt);
}

