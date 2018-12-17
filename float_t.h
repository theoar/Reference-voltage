/*
 * float_t.h
 *
 *  Created on: 09-11-2017
 *      Author: theoar
 */

#ifndef FLOAT_T_H_
#define FLOAT_T_H_

#include <inttypes.h>

typedef struct FLOATT{
	int64_t Data;
	static const uint16_t Float_tMultipler = 10000;
} Float_t;

void fset(Float_t *Float, int64_t Int, uint16_t ValueFloat);
void fset(Float_t * First, const Float_t * Second);

void fadd(Float_t *Float, int64_t Value);
void fminus(Float_t *Float, int64_t Value);
void fdiv(Float_t *Float, int64_t Value);
void fmultiple(Float_t *Float, int64_t Value);

void fadd(Float_t *First, const Float_t  *Second);
void fminus(Float_t *First, const Float_t  *Second);
void fdiv(Float_t *First, const Float_t  *Second);
void fmultiple(Float_t *First, const Float_t  *Second);

Float_t fadd_c(const Float_t *First, const Float_t  *Second);
Float_t fminus_c(const Float_t *First, const Float_t  *Second);
Float_t fdiv_c(const Float_t *First, const Float_t  *Second);
Float_t fmultiple_c(const Float_t *First, const Float_t  *Second);

Float_t fadd_c(const Float_t *Float, int64_t Value);
Float_t fminus_c(const Float_t *Float, int64_t Value);
Float_t fdiv_c(const Float_t *Float, int64_t Value);
Float_t fmultiple_c(const Float_t *Float, int64_t Value);

int32_t f_get_integer(const Float_t *First);
uint16_t f_get_floated(const Float_t *First);
uint32_t f_get_mod(const Float_t * Value);

bool f_is_greater(const Float_t *What, int64_t Than);
bool f_is_lesser(const Float_t *What, int64_t Than);
bool f_is_equal(const Float_t *What, int64_t To);
bool f_is_different(const Float_t *What, int64_t To);

bool f_is_greater(const Float_t *What, const Float_t * Than);
bool f_is_lesser(const Float_t *What, const Float_t * Than);
bool f_is_equal(const Float_t *What, const Float_t * To);
bool f_is_different(const Float_t *What, const Float_t * To);

bool f_is_zero(const Float_t *What);

uint8_t f_get_integer_digits(const Float_t *Value);
uint8_t f_get_floated_digits(const Float_t *Value);

Float_t f_single_step(void);


#endif /* FLOAT_T_H_ */
