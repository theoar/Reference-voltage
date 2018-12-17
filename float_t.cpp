/*
 * float_t.c
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#include "float_t.h"

void fset(Float_t *Float, int64_t Int, uint16_t ValueFloat)
{
	Float->Data = (Int * Float_t::Float_tMultipler) + ((Int >= 0) ? ValueFloat : ValueFloat*(-1));
}

void fset(Float_t * First, const Float_t * Second)
{
	First->Data = Second->Data;
}

void fadd(Float_t *Float, int64_t Value)
{
	Float->Data += Value;
}

void fminus(Float_t *Float, int64_t Value)
{
	Float->Data -= Value;
}

void fdiv(Float_t *Float, int64_t Value)
{
	Float->Data *= Float_t::Float_tMultipler;
	Float->Data /= Value;
	Float->Data /= Float_t::Float_tMultipler;
}

void fmultiple(Float_t *Float, int64_t Value)
{
	Float->Data *= Value * Float_t::Float_tMultipler;
	Float->Data /= Float_t::Float_tMultipler;
}

void fadd(Float_t *First, const Float_t *Second)
{
	First->Data += Second->Data;
}

void fminus(Float_t *First, const Float_t *Second)
{
	First->Data -= Second->Data;
}

void fdiv(Float_t *First, const Float_t *Second)
{
	First->Data *= Float_t::Float_tMultipler;
	First->Data /= Second->Data;
}
void fmultiple(Float_t *First, const Float_t *Second)
{
	First->Data *= Second->Data;
	First->Data /= Float_t::Float_tMultipler;
}

Float_t fadd_c(const Float_t *First, const Float_t  *Second)
{
	Float_t Output = *First;
	fadd(&Output, Second);
	return Output;
}

Float_t fminus_c(const Float_t *First, const Float_t  *Second)
{
	Float_t Output = *First;
	fminus(&Output, Second);
	return Output;
}

Float_t fdiv_c(const Float_t *First, const Float_t  *Second)
{
	Float_t Output = *First;
	fdiv(&Output, Second);
	return Output;
}

Float_t fmultiple_c(const Float_t *First, const Float_t  *Second)
{
	Float_t Output = *First;
	fmultiple(&Output, Second);
	return Output;
}

Float_t fadd_c(const Float_t *Float, int64_t Value)
{
	Float_t Output = *Float;
	fadd(&Output, Value);

	return Output;
}

Float_t fminus_c(const Float_t *Float, int64_t Value)
{
	Float_t Output = *Float;
	fminus(&Output, Value);

	return Output;
}

Float_t fdiv_c(const Float_t *Float, int64_t Value)
{
	Float_t Output = *Float;
	fdiv(&Output, Value);

	return Output;
}

Float_t fmultiple_c(const Float_t *Float, int64_t Value)
{
	Float_t Output = *Float;
	fmultiple(&Output, Value);

	return Output;
}


bool f_is_greater(const Float_t *What, int64_t Than)
{
	return What->Data > Than * Float_t::Float_tMultipler;
}

bool f_is_lesser(const Float_t *What, int64_t Than)
{
	return What->Data < Than * Float_t::Float_tMultipler;
}
bool f_is_equal(const Float_t *What, int64_t To)
{
	return What->Data == To * Float_t::Float_tMultipler;
}

bool f_is_different(const Float_t *What, int64_t To)
{
	return What->Data != To * Float_t::Float_tMultipler;
}

bool f_is_greater(const Float_t *What, const Float_t * Than)
{
	return What->Data > Than->Data;
}

bool f_is_lesser(const Float_t *What, const Float_t * Than)
{
	return What->Data < Than->Data;
}

bool f_is_equal(const Float_t *What, const Float_t * To)
{
	return What->Data == To->Data;
}

bool f_is_different(const Float_t *What, const Float_t * To)
{
	return What->Data != To->Data;
}

int32_t f_get_integer(const Float_t *First)
{
	return (int32_t)((First->Data/(int64_t)Float_t::Float_tMultipler));
}
uint16_t f_get_floated(const Float_t *First)
{
	if(First->Data<0)
		return (First->Data*-1) % Float_t::Float_tMultipler;

	return (uint16_t)((First->Data) % (int64_t)Float_t::Float_tMultipler);
}

uint32_t f_get_mod(const Float_t * Value)
{
	if(Value->Data<0)
		return (Value->Data)*-1;

	return Value->Data;
}

uint8_t f_get_integer_digits(const Float_t *Value)
{
	uint16_t X = f_get_integer(Value);
	uint8_t Counter = 0;
	while(X!=0)
	{
		Counter++;
		X/=10;
	}

	return Counter;
}

uint8_t f_get_floated_digits(const Float_t *Value)
{
	uint8_t Counter = 0;
	uint32_t X = f_get_floated(Value);
	while((X%Float_t::Float_tMultipler)!=0)
	{
		Counter++;
		X*=10;
	}

	return Counter;
}

bool f_is_zero(const Float_t *What)
{
	return What->Data==0;
}

Float_t f_single_step(void)
{
	Float_t X;
	fset(&X, 0, 1);

	return X;
}

