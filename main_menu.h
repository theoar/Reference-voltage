/*
 * main_menu.h
 *
 *  Created on: 22-11-2017
 *      Author: theoar
 */

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "lowermenu.h"
#include "options.h"

typedef struct MAINMENU
{
	Options *LowerMenuList;
	Variable *VarValue;
	LowerMenu *CurrentLowerMenu = NULL;

	uint8_t Index = 0;
	LowerMenu *LowerMenus[5];

	uint8_t LmCount = 0;

} MainMenu;

void mm_add_variable(MainMenu *M, Variable *Var);
void mm_add_options(MainMenu *M, Options *Opt);
void mm_add_lower_menu(MainMenu *M, LowerMenu *LM, const char *MenuName);

void mm_next(MainMenu *M);
void mm_prev(MainMenu *M);

void mm_display(MainMenu *M);

uint8_t mm_handle_int_action(MainMenu *M, uint8_t Action);

uint8_t mm_handle_action(MainMenu *M, uint8_t Action);

void mm_display_main(MainMenu *M)
{
	M->Index = 0;
	variable_display(M->VarValue);
}

void mm_display(MainMenu *M)
{
	if(M->Index==0)
		variable_display(M->VarValue);
	else
		options_display(M->LowerMenuList);
}

void mm_next(MainMenu *M)
{
	M->Index++;
	if(M->Index>1)
		M->Index = 0;

	mm_display(M);
}


void mm_prev(MainMenu *M)
{
	M->Index--;
	if(M->Index==255)
		M->Index=1;

	mm_display(M);
}

uint8_t mm_handle_action(MainMenu *M, uint8_t Action)
{
	uint8_t X = 0;
	if(M->CurrentLowerMenu!=NULL)
	{
		X =  lm_handle_action(M->CurrentLowerMenu, Action);

		if(X>0)
		{
			M->CurrentLowerMenu = NULL;
			M->Index = 0;
			mm_display(M);
		}
	}
	else
	{
		uint8_t ToWho = 0;

		if (M->Index==1 && Action != IO_ACTION_POS_LEFT && Action != IO_ACTION_POS_RIGHT)
			ToWho = 1;
		else if ( (Action == IO_ACTION_POS_LEFT || Action == IO_ACTION_POS_RIGHT)  &&  (M->Index==1 || !variable_is_actived_edit(M->VarValue)))
			ToWho = 2;
		if (ToWho == 2)
		{
			switch (Action)
			{
			case IO_ACTION_POS_LEFT:
				mm_prev(M);
				break;
			case IO_ACTION_POS_RIGHT:
				mm_next(M);
				break;
			}
		}

		if (ToWho == 0)
			X =  variable_handle_action(M->VarValue, Action);
		if (ToWho == 1)
			X = mm_handle_int_action(M, options_handle_action(M->LowerMenuList, Action));
	}

	return X;
}

void mm_add_variable(MainMenu *M, Variable *Var)
{
	M->VarValue = Var;
}

void mm_add_options(MainMenu *M, Options *Opt)
{
	M->LowerMenuList = Opt;
}

void mm_add_lower_menu(MainMenu *M, LowerMenu *LM, const char *MenuName)
{
	options_add_option(M->LowerMenuList, MenuName, M->LmCount+1);
	M->LowerMenus[M->LmCount] = LM;
	M->LmCount++;
}

uint8_t mm_handle_int_action(MainMenu *M, uint8_t Action)
{
	if(Action==0)
		return 0;

	M->CurrentLowerMenu = M->LowerMenus[Action-1];
	M->LowerMenus[Action-1]->Index = 0;
	M->LowerMenus[Action-1]->ActionList[0].Index = 0;

	lm_display(M->CurrentLowerMenu);

	return 0;
}

#endif /* MAIN_MENU_H_ */
