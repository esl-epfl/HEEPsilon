#include "fxp.h"
#include <stdio.h>
#include <stdint.h>
#include "math.h"

fxp int2fxp(int value)
{
	//Saturation values
	if(value >= (1 << FXP_INTEGER_BITS))
		return FXP_MAX;
	if(value <= (-(1 << FXP_INTEGER_BITS)))
		return FXP_MIN;

	//Naive iterative computation
	return (value << FXP_DECIMAL_BITS);
}

int fxp2int(fxp value)
{
	fxp temp_value = (value >= 0 ? value : -value);

	//Extract integer part
	int result = (temp_value >> (FXP_DECIMAL_BITS));

	//Round-up if necessary
	if(temp_value & (1 << (FXP_DECIMAL_BITS-1)))
		result++;

	//Re-adjust sign
	return (value >= 0 ? result : -result);
}

fxp float2fxp(float value)
{
	int bit;
	float temp_value, step;
	fxp result;

	//Saturation values
	if(value >= ((int)(1 << FXP_INTEGER_BITS)))
		return FXP_MAX;
	if(value <= ((int)(-(1 << FXP_INTEGER_BITS))))
		return FXP_MIN;

	temp_value = (value >= 0 ? value : -value);
	step = 1;
	result = 0;

	//Iterative naive transformation
	for(bit=1; bit<FXP_INTEGER_BITS; bit++)
		step*=2;
	for(bit = FXP_TYPE_SIZE-2; bit>=0; bit--) {
		if(step <= temp_value) {
			result += (1 << bit);
			temp_value -= step;
		}
		step /= 2.0;
	}

	//Re-adjust sign
	return (value >= 0 ? result : -result);
}

float fxp2float(fxp value)
{
	fxp temp_value = (value >= 0 ? value : -value);
	int bit;
	float result, step;

	//Iterative naive transformation
	result = 0;
	for(bit = 0; bit < FXP_DECIMAL_BITS; bit++) { //Decimal part
		if(temp_value & (1 << bit)) result += 1.0;
		result /= 2.0;
	}
	step = 1;
	for(bit = 0; bit < FXP_INTEGER_BITS; bit++) { //Integer part
		if(temp_value & (1 << (bit + FXP_DECIMAL_BITS)))
			result += step;
		step*=2;
	}

	//Re-adjust sign
	return (value >= 0 ? result : -result);
}

fxp double2fxp(double value)
{
	int bit;
	double temp_value, step;
	fxp result;

	//Saturation values
	if(value >= ((int)(1 << FXP_INTEGER_BITS)))
		return FXP_MAX;
	if(value <= ((int)(-(1 << FXP_INTEGER_BITS))))
		return FXP_MIN;


	temp_value = (value >= 0 ? value : -value);
	step = 1;
	result = 0;

	//Iterative naive transformation
	for(bit=1; bit<FXP_INTEGER_BITS; bit++)
		step*=2;
	for(bit = FXP_TYPE_SIZE-2; bit>=0; bit--) {
		if(step <= temp_value) {
			result += (1 << bit);
			temp_value -= step;
		}
		step /= 2.0;
	}

	//Re-adjust sign
	return (value >= 0 ? result : -result);
}

double fxp2double(fxp value)
{
	fxp temp_value = (value >= 0 ? value : -value);
	int bit;
	double result, step;

	//Iterative naive transformation
	result = 0;
	for(bit = 0; bit < FXP_DECIMAL_BITS; bit++) { //Decimal part
		if(temp_value & (1 << bit)) result += 1.0;
		result /= 2.0;
	}
	step = 1;
	for(bit = 0; bit < FXP_INTEGER_BITS; bit++) { //Integer part
		if(temp_value & (1 << (bit + FXP_DECIMAL_BITS)))
			result += step;
		step*=2;
	}

	//Re-adjust sign
	return (value >= 0 ? result : -result);
}

// Faster implementation
fxp fxp_mult(fxp a, fxp b)
{
	int64_t product = (int64_t)a * b;
	// product = product + (1 << (FXP_DECIMAL_BITS-1));// Better accuracy with it
	product >>= FXP_DECIMAL_BITS;

	return product;
}

fxp fxp_mult_mix(fxp a, fxp b, int fxp_dec_bits)
{
	int64_t product = (int64_t)a * b;
	int64_t res = product >> fxp_dec_bits;

	if(product > FXP_INT_MAX || product < FXP_INT_MIN)
	{
		printBits(sizeof(product), &product);
	}

	// printBits(sizeof(a), &a);
	// printBits(sizeof(b), &b);
	// printBits(sizeof(product), &product);
	// printBits(sizeof(res), &res);

	return (int32_t) res;
}

//assumes little endian
void printBits(const int size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

fxp fxp_div(fxp a, fxp b)
{
	fxp bit, solution, temp;
	solution = FXP_ZERO;
	temp = FXP_ZERO;
	if (b == FXP_ZERO) return FXP_MAX;

	// Store result sign and put everything in positive
	int negative_result = (a < FXP_ZERO) ^ (b < FXP_ZERO);
	if(a < FXP_ZERO) a = -a;
	if(b < FXP_ZERO) b = -b;

	//Reduce b (<= 1) so the check in the multiplication does not overflow
	bit = (1 << (FXP_TYPE_SIZE - 2));
	while(b > FXP_ONE) {
		a >>= 1;
		b >>= 1;
	}

	//Iterative algorithm
	while (bit != FXP_ZERO) {
		temp = solution + bit;
		if (fxp_mult(temp, b) <= a)
			solution = temp;
		bit = (bit >> 1);
	}

	//Rearrange sign
	if(negative_result) solution = -solution;

	return solution;
}
