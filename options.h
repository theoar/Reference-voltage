/*
 * option.h
 *
 *  Created on: 21-11-2017
 *      Author: theoar
 */

#ifndef OPTION_H_
#define OPTION_H_

#include <inttypes.h>

typedef struct OPTIONS
{
	char Title[16];
	char OptionNames[5][15];
	uint8_t AcceptEvent[5];

	uint8_t Count = 0;
	uint8_t Index = 0;

	char LeftArrow = '<';
	char RightArrow = '>';

} Options;


void options_add_title(Options * Opt, const char *Title);
void options_add_option(Options * Opt, const char *Name, uint8_t AcceptEvent);
void options_display_title(Options *Opt);
void options_display_option(Options *Opt);
void options_display(Options *Opt);
void options_go_left(Options *Opt);
void options_go_right(Options *Opt);
void options_go_first(Options *Opt);

uint8_t options_select_action(Options *Opt);
uint8_t options_handle_action(Options *Opt, uint8_t Action);



#endif /* OPTION_H_ */
