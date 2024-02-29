#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

#include <stdint.h>

uint16_t isqrt32(uint32_t in_ptr) 
{
	uint16_t mask = 1<<14;
	uint16_t result = 0;
	uint16_t temp = 0;

	if(in_ptr < 0) return 0;
	while(mask)
	{
		temp = result | mask;
		if((((uint32_t)temp)*((uint32_t)temp)) <= ((uint32_t)(in_ptr)))
			result = temp;
		mask >>= 1;
	}
	return result;
}

#endif // _CGRA_FUNCTION_H_