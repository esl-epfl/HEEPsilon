#ifndef __FXP_H__
#define __FXP_H__

#include <stdint.h>

#define FXP_TYPE_SIZE       32
#define FXP_INTEGER_BITS    16
#define	FXP_DECIMAL_BITS	(FXP_TYPE_SIZE - FXP_INTEGER_BITS - 1)

#define	FXP_MAX				((((1 << (FXP_TYPE_SIZE-2)) - 1) << 1) + 1)
#define FXP_MIN				((1 << (FXP_TYPE_SIZE - 1)) + 1)
#define FXP_INT_MAX			(FXP_MAX - ((1 << FXP_DECIMAL_BITS) - 1))
#define FXP_INT_MIN			(FXP_MIN + ((1 << FXP_DECIMAL_BITS) - 1))
#define FXP_ZERO			(0)
#define	FXP_ONE				(((int)1) << FXP_DECIMAL_BITS)
#define	FXP_MINUS_ONE		(((int) -1) << FXP_DECIMAL_BITS)

// int must be a 32-bit type and short a 16-bit one
#if FXP_TYPE_SIZE==32
typedef signed int fxp;
typedef signed long d_fxp;
#elif FXP_TYPE_SIZE==16
typedef signed short fxp;
typedef signed int d_fxp;
#elif FXP_TYPE_SIZE==8
typedef signed short fxp;
typedef signed int d_fxp;
#else
#error FXP_TYPE_SIZE must be defined either as 32
#endif

fxp int2fxp(int value);
int fxp2int(fxp value);
fxp float2fxp(float value);
float fxp2float(fxp value);
fxp double2fxp(double value);
double fxp2double(fxp value);
fxp fxp_mult(fxp a, fxp b);
fxp fxp_div(fxp a, fxp b);

fxp fxp_mult_mix(fxp a, fxp b, int fxp_dec_bits);
void printBits(const int size, void const * const ptr);

#endif // __FXP_H__
