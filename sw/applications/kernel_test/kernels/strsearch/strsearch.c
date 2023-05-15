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

static uint32_t cgra_imem_bitstream[CGRA_IMEM_SIZE] = {  0x0, 0x0, 0x0, 0x0, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x5a501fff, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc80000, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x26080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x4a180004, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x30080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x0, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x2a080001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x3b80000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x64080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x4a180004, 0x16080000, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa080000, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x5a080001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa80000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10080000, 0x51784000, 0x10b00000, 0x0, 0xa90000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x4b80000, 0x16100000, 0x0, 0x0, 0x0, 0xa90000, 0x40080000, 0x0, 0x40080000, 0x46800012, 0x40080000, 0x46800012, 0x40080000, 0x4680000b, 0x40080000, 0x46880009, 0x0, 0x4680000d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,   };
static uint32_t cgra_kmem_bitstream[CGRA_KMEM_SIZE] = {  0x0,  0xf013,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 };

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

	i_patlen_soft = kcom_getRand() % (DEBUG_STR_LEN - 1 + 1) + 1;
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
		i_pattern_soft[i] =  kcom_getRand() % (255 - 1 + 1) + 1;
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
	cgra_input[3][1] = (uint32_t)(i_patlen_cgra -1-4);

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