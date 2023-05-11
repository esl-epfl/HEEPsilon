/*
                              *******************
******************************* C SOURCE FILE *******************************
**                            *******************                          **
**                                                                         **
** project  : CGRA-X-HEEP                                                  **
** filename : strsearch.c                                                 **
** version  : 1                                                            **
** date     : 2023-04-24                                                       **
**                                                                         **
*****************************************************************************
**                                                                         **
** Copyright (c) EPFL                                                      **
** All rights reserved.                                                    **
**                                                                         **
*****************************************************************************
*/

/***************************************************************************/
/***************************************************************************/

/**
* @file   strsearch.c
* @date   2023-04-24
* @brief  A description of the kernel...
*
*/

#define _STRSEARCH_C

/****************************************************************************/
/**                                                                        **/
/*                             MODULES USED                                 */
/**                                                                        **/
/****************************************************************************/
#include <stdint.h>
#include <string.h>

#include "strsearch.h"
#include "data/function.h"

/****************************************************************************/
/**                                                                        **/
/*                        DEFINITIONS AND MACROS                            */
/**                                                                        **/
/****************************************************************************/

#define CGRA_COLS       4
#define IN_VAR_DEPTH    2
#define OUT_VAR_DEPTH   1





#define DEBUG_STR_LEN   50





/****************************************************************************/
/**                                                                        **/
/*                      PROTOTYPES OF LOCAL FUNCTIONS                       */
/**                                                                        **/
/****************************************************************************/

static void        config  (void);
static void        software(void);
static uint32_t    check   (void);

/****************************************************************************/
/**                                                                        **/
/*                            GLOBAL VARIABLES                              */
/**                                                                        **/
/****************************************************************************/

static uint32_t cgra_imem_bitstream[CGRA_IMEM_SIZE] = {  0x0, 0x0, 0x0, 0x0, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x0, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa80000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10b00000, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x0, 0x0, 0xa90000, 0x40080000, 0x0, 0x40080000, 0x46800012, 0x40080000, 0x46800012, 0x40080000, 0x4680000b, 0x40080000, 0x46880009, 0x0, 0x4680000d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  };
static uint32_t cgra_kmem_bitstream[CGRA_KMEM_SIZE] = {  0x0,  0xf012,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 };

static int32_t cgra_input[CGRA_COLS][IN_VAR_DEPTH]     __attribute__ ((aligned (4)));
static int32_t cgra_output[CGRA_COLS][OUT_VAR_DEPTH]   __attribute__ ((aligned (4)));

static int32_t	i_patlen_soft;
static int32_t	i_patlen_cgra;
static int32_t	i_skip2_soft;
static int32_t	i_skip2_cgra;
static uint32_t	i_pattern_soft[DEBUG_STR_LEN];
static uint32_t	i_pattern_cgra[DEBUG_STR_LEN];

static int32_t	o_return_soft;
static int32_t	o_return_cgra;



/****************************************************************************/
/**                                                                        **/
/*                           EXPORTED VARIABLES                             */
/**                                                                        **/
/****************************************************************************/

extern kcom_kernel_t strs_kernel = {
    .kmem   = cgra_kmem_bitstream,
    .imem   = cgra_imem_bitstream,
    .col_n  = CGRA_COLS,
    .in_n   = IN_VAR_DEPTH,
    .out_n  = OUT_VAR_DEPTH,
    .input  = cgra_input,
    .output = cgra_output,
    .config = config,
    .func   = software,
    .check  = check,
    .name   = "Strsearch",
};

/****************************************************************************/
/**                                                                        **/
/*                            LOCAL FUNCTIONS                               */
/**                                                                        **/
/****************************************************************************/

void config()
{
    static uint32_t test_idx = 0;

    static uint32_t test_patterns[ITERATIONS_PER_KERNEL][DEBUG_STR_LEN] = {
        { 197, 55, 06, 155, 142, 132, 68, 248, 147, 143, 130, 248, 230, 109, 01, 86, 15, 233, 251, 71, 57, 68, 51, 38, 217, 119, 23, 89, 126, 77, 44, 108, 12, 183, 29, 71, 30, 240, 147, 78, 167, 11, 156, 71, 64, 186, 188, 234, 55, 248},
        { 80 , 60, 51, 77,  87, 46, 56, 70,  116,  82,  127, 42, 95, 59, 39, 33, 40, 94,  102,  125,  100, 76, 88,  113, 68,  107, 77, 38, 51, 60,  121, 98, 42,  127, 95, 49, 92,  112,  117, 42, 32, 41, 44, 83,  108, 83, 46, 83, 82, 68, 0},
        { 52,  80,  92,  86,  50,  81, 54,  111,  118,  87, 47,  68, 90, 65, 85,  105,  122, 50, 98, 82, 75, 42,  125, 66, 53, 69, 72,  129,  125, 86,  101,  111,  106,  103,  109, 45,  126,  105, 52,  114, 91, 49,  116, 39, 51, 84, 86, 83, 80, 93},
        { 52,  80,  92,  86,  50,  81, 54,  111,  118,  87, 47,  68, 90, 65, 85,  105,  122, 50, 98, 82, 75, 42,  125, 66, 53, 69, 72,  129,  125, 86,  101,  111,  106,  103,  109, 45,  126,  80, 52,  114, 91, 49,  116, 39, 51, 84, 86, 83, 80, 93},
        { 6 ,82 ,13 ,11 ,82 , 9, 14 , 5, 82, 9}
    }; 

    static uint32_t test_patlens[ITERATIONS_PER_KERNEL] = { 30, 49, 49, 49, 9 };

	// i_patlen_soft = kcom_getRand() % (DEBUG_STR_LEN - 1 + 1) + 1;
    i_patlen_soft = test_patlens[test_idx];
    i_patlen_cgra = i_patlen_soft;
    PRINTDBG("patlen_cgra = %d (%08x)\n", i_patlen_cgra,i_patlen_cgra);

	i_skip2_soft = 123;
	i_skip2_cgra = i_skip2_soft;

    for(int i = 0; i < DEBUG_STR_LEN; i++ )
	{   
        PRINTDBG(" %03d \t", i);
    }
    PRINTDBG("\n");

    for(int i = 0; i < DEBUG_STR_LEN; i++ )
	{
		i_pattern_soft[i] = test_patterns[test_idx][i];
		i_pattern_cgra[i] = i_pattern_soft[i];
        if( i == i_patlen_cgra - 1 ){
            PRINTDBG("[%03d]\t",i_pattern_cgra[i]);
        }
        else{
            PRINTDBG(" %03d \t",i_pattern_cgra[i]);
        }
	}

    PRINTDBG("\n");

	cgra_input[1][0] = (uint32_t)i_patlen_cgra;
	cgra_input[1][1] = (uint32_t)i_skip2_cgra;

	cgra_input[2][0] = (uint32_t)lowervec;
	cgra_input[2][1] = (uint32_t)lowerc(i_pattern_cgra[i_patlen_cgra - 1]);

	cgra_input[3][0] = (uint32_t)i_pattern_cgra;
	cgra_input[3][1] = (uint32_t)(i_patlen_cgra -2);


    test_idx++;

}

void software(void) 
{
    o_return_soft = strsearch( i_patlen_soft, i_skip2_soft, i_pattern_cgra );
}

uint32_t check(void) 
{
    uint32_t errors = 0;
    
	o_return_cgra = cgra_output[1][0];


#if PRINT_CGRA_RESULTS
    PRINTF("------------------------------\n");
    for( uint8_t c = 0; c < CGRA_COLS; c ++)
    {
        for( uint8_t r = 0; r < OUT_VAR_DEPTH; r++ )
        {
            PRINTF("[%d][%d]:%08x\t\t",c,r,cgra_output[c][r]);
        }
        PRINTF("\n");
    }
#endif //PRINT_CGRA_RESULTS


#if PRINT_RESULTS
        PRINTF("\nCGRA\t\tSoft\n");
#endif

    for( int i = 0; i < 1; i++ )
    {
#if PRINT_RESULTS
        PRINTF("%d\t%d\t%s\n",
        o_return_cgra,
        o_return_soft,
        (o_return_cgra != o_return_soft) ? "Wrong!\n\n" : "\n\n"
        );
#endif

        if (o_return_cgra != o_return_soft) {
            errors++;
        }
    }
    return errors;
}

/****************************************************************************/
/**                                                                        **/
/**                                EOF                                     **/
/**                                                                        **/
/****************************************************************************/