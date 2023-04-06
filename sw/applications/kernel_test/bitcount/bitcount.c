#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "bitcount.h"
#include "cgra/cgra_bitstream.h"
#include "cgra/function.h"

// TEST PARAMETERS
#define REPETITIONS 10

// cgra variables
#define CGRA_COLS   4
#define IN_VARS     1
#define OUT_VARS    1

kcom_func_ret_t bitcount(void);
kcom_func_ret_t check(void);

int32_t cgra_input  [CGRA_COLS][IN_VARS]    __attribute__ ((aligned (4)));
int32_t cgra_output [CGRA_COLS][OUT_VARS]   __attribute__ ((aligned (4)));

int32_t sw_res = {0};

extern kcom_kernel_t bitc_kernel = {
    .kmem       = cgra_kmem_bitstream,
    .imem       = cgra_imem_bitstream,
    .col_n      = CGRA_COLS,
    .input      = cgra_input,
    .output     = cgra_output,
    .check      = check,
    .function   = bitcount
};

kcom_func_ret_t bitcount(void) 
{
    int32_t x = kcom_getRand() % UINT_MAX;

    PRINTF("x  = %d\n", x);
    cgra_input[3][0] = (int32_t)x;
    unsigned sw_res = bit_count ( x );
    PRINTF("sw = %d\n", sw_res);
    return KCOM_FUNC_RET_OK;
}

kcom_func_ret_t check(void) 
{
    int errors = 0;
    PRINTF("Bitcount!\n");
    
    PRINTF("cg = %d\n", cgra_output[0][0]);

    //for(int r=0 ; r < REPETITIONS ; r++){
        unsigned cgra_res = cgra_output[0][0];
        if (cgra_res != sw_res) {
            errors++;
        }
    //}

    return errors;
}
