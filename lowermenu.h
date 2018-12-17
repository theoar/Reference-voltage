/*
 * lowermenu.h
 *
 *  Created on: 21-11-2017
 *      Author: theoar
 */

#ifndef LOWERMENU_H_
#define LOWERMENU_H_

#include "variable.h"
#include "options.h"

typedef struct LOWERMENU{
	Variable *VarList[5];
	Options *ActionList;

	uint8_t Count = 0;
	uint8_t Index = 0;

} LowerMenu;

void lm_add_variable(LowerMenu *M, Variable *Var);
void lm_add_action_menu(LowerMenu *M, Options *Opt);

uint8_t lm_handle_action(LowerMenu *M, uint8_t Action);

void lm_display(LowerMenu *M);
void lm_go_first(LowerMenu *M);

void lm_next(LowerMenu *M, uint8_t Action);
void lm_prev(LowerMenu *M, uint8_t Action);

void lm_accept(LowerMenu *M, uint8_t Action);

#endif /* LOWERMENU_H_ */
