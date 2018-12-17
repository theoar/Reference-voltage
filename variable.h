/*
 * variable.h
 *
 *  Created on: 13-11-2017
 *      Author: theoar
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "float_t.h"
#include "io_events.h"

#include <stdlib.h>
#include <avr/pgmspace.h>

typedef struct VARIABLE
{
	Float_t Value;
	Float_t Min;
	Float_t Max;
	Float_t Step;

	Float_t **List;
	uint8_t ListCount = 0;
	uint8_t ListIndex = 0;

	char Prefix[8];
	char Sufix[6];
	char Title[16];

	uint8_t Len_Var;
	uint8_t Len_Prefix;
	uint8_t Len_Sufix;

	uint8_t Y_Pos = 1;
	uint8_t X_Pos = 0;

	int8_t CurrentPos = -1;
	uint8_t CurrentX;

	uint8_t IntegerDigits;
	uint8_t FloatedDigits;

	uint8_t AccpetEvent = 0;

	bool ReadOnly = false;

} Variable;

uint8_t variable_next_left(Variable* Var);
uint8_t variable_next_right(Variable* Var);

void variable_calculate_position(Variable *Var);

void variable_increment(Variable* Var);
void variable_decrement(Variable* Var);

void variable_set_values(Variable* Var, Float_t * Value, Float_t * Max,  Float_t * Min, Float_t * Step);
void variable_set_list(Variable *Var, Float_t ** List, const uint8_t Count);
uint8_t variable_get_index(Variable * Var);
void variable_set_index(Variable * Var, uint8_t Index);
void variable_set_prefix(Variable *Var, const  char *Prefix);
void variable_set_sufix(Variable* Var,const  char *Sufix);
void variable_set_title(Variable* Var, const char *Title);
void variable_set_acept_event(Variable *Var, uint8_t Event);

void variable_set_value(Variable* Var, const Float_t * Value);
void variable_set_value(Variable *Var, const Float_t Value);

void variable_display_sufix(Variable *Var);
void variable_display_value(Variable* Var);
void variable_display_prefix(Variable* Var);
void variable_display_title(Variable *Var);
void variable_display_variable(Variable* Var);
void variable_display(Variable *Var);

void variable_goto_current(Variable* Var);
void variable_goto_prefix(Variable *Var);
void variable_goto_value(Variable *Var);
void variable_goto_sufix(Variable *Var);
void variable_goto_title(Variable *Var);

void variable_active(Variable* Var);
void variable_active_edit(Variable *Var);
void variable_deactive_edit(Variable *Var);
bool variable_is_actived_edit(Variable *Var);
void variable_accept(Variable *Var);

void variable_readonly(Variable *Var, bool ReadOnly);
bool variable_is_readonly(Variable *Var);

uint8_t variable_handle_action(Variable *Var, uint8_t Action);




#endif /* VARIABLE_H_ */
