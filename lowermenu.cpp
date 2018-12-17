/*
 * lowermenu.cpp
 *
 *  Created on: 21-11-2017
 *      Author: theoar
 */
#include "lowermenu.h"
#include "actions.h"

#include <stdio.h>

void lm_add_variable(LowerMenu *M, Variable *Var)
{
	M->VarList[M->Count] = Var;
	M->Count++;
}

void lm_go_first(LowerMenu *M)
{
	M->Index = 0;
	lm_display(M);
}

void lm_add_action_menu(LowerMenu *M, Options *Opt)
{
	M->ActionList = Opt;
}

void lm_next(LowerMenu *M)
{
	M->Index++;
	if (M->Index > M->Count)
		M->Index = 0;


	lm_display(M);
}

void lm_prev(LowerMenu *M)
{
	M->Index--;
	if (M->Index == 255)
		M->Index = M->Count;


	lm_display(M);
}

uint8_t lm_handle_action(LowerMenu *M, uint8_t Action)
{
	uint8_t ToWho = 0;

	if (M->Index==M->Count && Action != IO_ACTION_POS_LEFT && Action != IO_ACTION_POS_RIGHT)
		ToWho = 1;
	else
		if ( (Action == IO_ACTION_POS_LEFT || Action == IO_ACTION_POS_RIGHT)  && (M->Index==M->Count  || !variable_is_actived_edit(M->VarList[M->Index]) ))
		ToWho = 2;

	if(ToWho == 2)
	{

		switch (Action)
		{
		case IO_ACTION_POS_LEFT:
			lm_prev(M);
			break;
		case IO_ACTION_POS_RIGHT:
			lm_next(M);
			break;
		}
		return 0;
	}

	if(ToWho == 0)
		return variable_handle_action(M->VarList[M->Index], Action);
	if(ToWho == 1)
		return options_handle_action(M->ActionList, Action);
}

void lm_display(LowerMenu *M)
{
	if(M->Index==M->Count)
		options_display(M->ActionList);
	if(M->Index<M->Count)
		variable_display(M->VarList[M->Index]);
}
