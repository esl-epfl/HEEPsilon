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

void        config  (void);
void        bitcount(void);
uint32_t    check   (void);

int32_t cgra_input  [CGRA_COLS][IN_VARS]    __attribute__ ((aligned (4)));
int32_t cgra_output [CGRA_COLS][OUT_VARS]   __attribute__ ((aligned (4)));

int32_t sw_res, cgra_res, x;

extern kcom_kernel_t bitc_kernel = {
    .kmem   = cgra_kmem_bitstream,
    .imem   = cgra_imem_bitstream,
    .col_n  = CGRA_COLS,
    .input  = cgra_input,
    .output = cgra_output,
    .config = config,
    .func   = bitcount,
    .check  = check,
};


void config()
{
    x = kcom_getRand() % UINT_MAX;
    cgra_input[3][0] = x;
}

void bitcount(void) 
{
    sw_res = bit_count ( x );
}

uint32_t check(void) 
{
    uint32_t errors = 0;
    
    PRINTF("Bitcount!\n");

    cgra_res = cgra_output[0][0];
    if (cgra_res != sw_res) {
        errors++;
    }
    return errors;
}
