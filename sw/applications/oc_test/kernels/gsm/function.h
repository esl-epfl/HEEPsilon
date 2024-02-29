#ifndef _CGRA_FUNCTION_H_
#define _CGRA_FUNCTION_H_

#include <stdint.h>
#include <stdio.h>

# define GSM_ABS(a) ((a) < 0 ? ((a) == MIN_WORD ? MAX_WORD : -(a)) : (a))
#define MIN_WORD    ((-32767)-1)
#define MAX_WORD    ( 32767)

int32_t gsm(int32_t *d){

    int32_t dmax = 0;
    int32_t temp = 0;

    for (int k = 0; k <= 39; k++) {
        temp = d[k];
        temp = GSM_ABS( temp );
        if (temp > dmax) dmax = temp;
    }

    return dmax;
}


#endif // _CGRA_FUNCTION_H_